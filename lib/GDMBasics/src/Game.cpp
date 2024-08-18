//
// Created by elly_sparky on 23/01/24.
//

#include "GDMBasics.h"

namespace mate
{
std::shared_ptr<Game> Game::_instance = nullptr;

void Game::setWindowView(sf::View view, u_int id) const
{
    for (const auto &target : _secondary_targets)
    {
        if (target.id == id)
        {
            target.target->setView(view);
        }
    }
}

std::shared_ptr<Game> Game::getGame()
{
    // std::lock_guard<std::mutex> lock(mutex); //Uncomment when I start working with threads, maybe

    if (!_instance)
    {
        _instance = std::shared_ptr<Game>(new Game());
        // Todo: set default values for screen size and game name
        _instance->addRoom(std::move(std::make_shared<Room>()));
    }
    return _instance;
}

std::shared_ptr<Game> Game::getGame(int winWidth, int winHeight, const std::string &gameName,
                                    std::shared_ptr<Room> mainRoom)
{
    // std::lock_guard<std::mutex> lock(mutex); //Uncomment when I start working with threads, maybe
    if (!_instance)
    {
        _instance = std::shared_ptr<Game>(new Game());
        _instance->_main_render_target.target->setSize(sf::Vector2u(winWidth, winHeight));
        _instance->_main_render_target.target->setTitle(gameName);
        _instance->_rooms.push_back(mainRoom);
        _instance->_active_room = std::move(mainRoom);
    }
    return _instance;
}

[[maybe_unused]] std::shared_ptr<Game> Game::getGame(int winWidth, int winHeight, const std::string &gameName,
                                                     std::list<std::shared_ptr<Room>> &roomsList)
{
    // std::lock_guard<std::mutex> lock(mutex); //Uncomment when I start working with threads, maybe
    if (!_instance)
    {
        _instance = std::shared_ptr<Game>(new Game());
        _instance->_main_render_target.target->setSize(sf::Vector2u(winWidth, winHeight));
        _instance->_main_render_target.target->setTitle(gameName);
        _instance->_rooms = roomsList;
        if (!roomsList.empty())
        {
            _instance->_active_room = roomsList.front();
        }
    }
    return _instance;
}

void Game::draw(const std::shared_ptr<const ord_sprite> &sprite, u_int id)
{
    if (id == 0)
    {
        _main_render_target.target->draw(sprite->sprite);
    }
    else
    {
        for (const auto &target : _secondary_targets)
        {
            target.target->draw(sprite->sprite);
        }
    }
}

u_int Game::addSecondaryTarget(sf::View view)
{
    render_target new_target;
    u_int id = new_target.id;
    new_target.target = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 400), "Game: Second Window");
    new_target.target->setView(view);
    _secondary_targets.push_back(std::move(new_target));
    return id;
}

[[maybe_unused]] void Game::switchRoom(int position)
{
    if (position < _rooms.size())
    {
        auto it = _rooms.begin();
        std::advance(it, position);
        _active_room = *it;
    }
}

[[noreturn]] void Game::gameLoop(bool isTesting)
{
    _main_render_target.target->setFramerateLimit(60);
    do
    {
        // Event Pooling
        sf::Event event{};
        while (_main_render_target.target->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                exit(EXIT_SUCCESS);
            case sf::Event::Resized:
                _active_room->resizeEvent();
                break;
            default:
                break;
            }
        }

        _active_room->curateTriggers();

        // Todo: Data loop
        _active_room->dataLoop();

        // Todo: Render Loop
        _main_render_target.target->clear();
        for (const auto &target : _secondary_targets)
        {
            target.target->clear();
        }

        _active_room->renderLoop();

        for (const auto &target : _secondary_targets)
        {
            target.target->display();
        }
        _main_render_target.target->display();

        // Todo: Secondary targets
    } while (_main_render_target.target->isOpen() && !isTesting);
    exit(0);
}
} // namespace mate
