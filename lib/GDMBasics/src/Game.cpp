//
// Created by elly_sparky on 23/01/24.
//

#include "GDMBasics.h"

namespace mate{
    std::shared_ptr<Game> Game::_instance = nullptr;

    std::shared_ptr<Game> Game::getGame() {
        //std::lock_guard<std::mutex> lock(mutex); //Uncomment when I start working with threads, maybe
        if(!_instance){
            _instance = std::shared_ptr<Game>(new Game());
            //Todo: set default values for screen size and game name
            _instance->addRoom(std::move(std::make_shared<Room>()));

        }
        return _instance;
    }

    std::shared_ptr<Game> Game::getGame(int winWidth, int winHeight, const std::string &gameName, std::shared_ptr<Room> mainRoom) {
        //std::lock_guard<std::mutex> lock(mutex); //Uncomment when I start working with threads, maybe
        if(!_instance){
            _instance = std::shared_ptr<Game>(new Game());
            _instance->_target.target->setSize(sf::Vector2u (winWidth, winHeight));
            _instance->_target.target->setTitle(gameName);
            _instance->_rooms.push_back(mainRoom);
            _instance->_active_room = std::move(mainRoom);
        }
        return _instance;
    }

    [[maybe_unused]]
    std::shared_ptr<Game> Game::getGame(int winWidth, int winHeight, const std::string &gameName, std::list<std::shared_ptr<Room>>& roomsList){
        //std::lock_guard<std::mutex> lock(mutex); //Uncomment when I start working with threads, maybe
        if(!_instance){
            _instance = std::shared_ptr<Game>(new Game());
            _instance->_target.target->setSize(sf::Vector2u (winWidth, winHeight));
            _instance->_target.target->setTitle(gameName);
            _instance->_rooms = roomsList;
            if(!roomsList.empty()) {
                _instance->_active_room = roomsList.front();
            }
        }
        return _instance;
    }

    [[maybe_unused]]
    void Game::switchRoom(int position) {
        if(position < _rooms.size()){
            auto it = _rooms.begin();
            std::advance(it, position);
            _active_room = *it;
        }
    }

    void Game::gameLoop(bool isTesting) {
        _target.target->setFramerateLimit(60);
        do {
            //Event Pooling
            sf::Event event{};
            while (_target.target->pollEvent(event))
            {
                switch (event.type) {
                    case sf::Event::Closed:
                        exit(EXIT_SUCCESS);
                    case sf::Event::Resized:
                        _active_room->ResizeEvent();
                        break;
                    default:
                        break;
                }
            }

            //Todo: Data loop
            _active_room->DataLoop();

            //Todo: Render Loop
            _target.target->clear();
            _active_room->RenderLoop();
            _target.printQueue.sort([](ord_sprite* a, ord_sprite* b){ return a->depth < b->depth; });

            for(ord_sprite* sp : _target.printQueue){
                _target.target->draw(sp->sprite);
            }

            _target.target->display();
        } while (_target.target->isOpen() && !isTesting);
        exit(0);
    }
}//mate