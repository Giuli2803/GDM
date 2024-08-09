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
            _instance->_main_render_target.target->setSize(sf::Vector2u (winWidth, winHeight));
            _instance->_main_render_target.target->setTitle(gameName);
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
            _instance->_main_render_target.target->setSize(sf::Vector2u (winWidth, winHeight));
            _instance->_main_render_target.target->setTitle(gameName);
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

    [[noreturn]]
    void Game::gameLoop(bool isTesting) {
        _main_render_target.target->setFramerateLimit(60);
        do {
            //Event Pooling
            sf::Event event{};
            while (_main_render_target.target->pollEvent(event))
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

            _trigger_manager.Curate();

            //Todo: Data loop
            _active_room->DataLoop();

            //Todo: Render Loop
            _main_render_target.target->clear();
            for (const auto& target : _secondary_targets){
                target.target->clear();
            }

            _active_room->RenderLoop();

            for (const auto& target : _secondary_targets){
                target.target->display();
            }
            _main_render_target.target->display();

            // Todo: Secondary targets
        } while (_main_render_target.target->isOpen() && !isTesting);
        exit(0);
    }
}//mate
