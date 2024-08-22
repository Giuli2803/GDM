//
// Created by elly_sparky on 23/01/24.
//

#include "GDMBasics.h"

namespace mate
{
std::shared_ptr<Game> Game::_instance = nullptr;

void Game::setWindowView(sf::View view_, u_int id_) const
{
    if(id_ == 0){
        _main_render_target.target->setView(view_);
        return;
    }
    for (const auto &target : _secondary_targets)
    {
        if (target.id == id_)
        {
            target.target->setView(view_);
            return;
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

std::shared_ptr<Game> Game::getGame(int win_width_, int win_height_, const std::string &game_name_,
                                    std::shared_ptr<Room> main_room_)
{
    // std::lock_guard<std::mutex> lock(mutex); //Uncomment when I start working with threads, maybe
    if (!_instance)
    {
        _instance = std::shared_ptr<Game>(new Game());
    }
    _instance->_main_render_target.target->setSize(sf::Vector2u(win_width_, win_height_));
    _instance->_main_render_target.target->setTitle(game_name_);
    _instance->_rooms.push_back(main_room_);
    _instance->_active_room = std::move(main_room_);
    return _instance;
}

[[maybe_unused]] std::shared_ptr<Game> Game::getGame(int win_width_, int win_height_, const std::string &game_name_,
                                                     std::list<std::shared_ptr<Room>> &rooms_list_)
{
    // std::lock_guard<std::mutex> lock(mutex); //Uncomment when I start working with threads, maybe
    if (!_instance)
    {
        _instance = std::shared_ptr<Game>(new Game());
    }
    _instance->_main_render_target.target->setSize(sf::Vector2u(win_width_, win_height_));
    _instance->_main_render_target.target->setTitle(game_name_);
    _instance->_rooms.merge(rooms_list_);
    if (!rooms_list_.empty())
    {
        _instance->_active_room = rooms_list_.front();
    }
    return _instance;
}

void Game::draw(const std::shared_ptr<const ord_sprite> &sprite_, u_int id_)
{
    if (id_ == 0)
    {
        _main_render_target.target->draw(sprite_->sprite);
    }
    else
    {
        for (const auto &target : _secondary_targets)
        {
            target.target->draw(sprite_->sprite);
        }
    }
}

u_int Game::addSecondaryTarget(sf::View view_)
{
    render_target new_target;
    u_int id = new_target.id;
    new_target.target = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 400), "Game: Second Window");
    new_target.target->setView(view_);
    _secondary_targets.push_back(std::move(new_target));
    return id;
}

[[maybe_unused]] void Game::switchRoom(int position_)
{
    if (position_ < _rooms.size())
    {
        auto it = _rooms.begin();
        std::advance(it, position_);
        _active_room = *it;
    }
}

[[noreturn]] void Game::gameLoop()
{
    _main_render_target.target->setFramerateLimit(60);
    do
    {
        run_single_frame();
    } while (_main_render_target.target->isOpen());
    exit(0);
}

void Game::run_single_frame()
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
}
} // namespace mate
