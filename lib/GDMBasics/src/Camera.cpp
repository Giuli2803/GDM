//
// Created by elly_sparky on 23/01/24.
//

#include "Camera.h"

namespace mate{
    Camera::Camera(const std::weak_ptr<Element>& parent)
    : Component(parent)
    {
        _view.setCenter(sf::Vector2f (0, 0));
        _view.setSize(sf::Vector2f (480, 360));
        _game_manager = mate::Game::getGame();
        if(auto _spt_game = _game_manager.lock()) {
            _spt_game->setWindowView(_view);
        }
        _aspect_ratio = 4.0f/3.0f;
    }

    Camera::~Camera(){
        if(auto _spt_game = _game_manager.lock()) {
            _spt_game->setWindowView();
        }
    }

    void Camera::Loop() {
        if(auto _spt_game = _game_manager.lock()){
            if (std::shared_ptr<Element> spt_parent = _parent.lock()) {
                _view.setCenter(spt_parent->getWorldPosition());
                _view.setRotation(spt_parent->getWorldRotation());

                //Todo: Update only once? Or redo just in case?
                _spt_game->setWindowView(_view);
            }
        }
    }

    void Camera::WindowResizeEvent() {
        auto _spt_game = _game_manager.lock();
        if (!_spt_game){
            return;
        }
        switch (_scale_type) {
            case RESCALE:
                break;
            case REVEAL:
                _view.setSize((float) _spt_game->getWindowSize().x, (float) _spt_game->getWindowSize().y);
                break;
            case LETTERBOX:
                float m_window_width = (float) _spt_game->getWindowSize().x;
                float m_window_height = (float) _spt_game->getWindowSize().y;
                float new_width = _aspect_ratio * m_window_height;
                float new_height = m_window_width / _aspect_ratio;
                float offset_width = (m_window_width - new_width) / 2.0f;
                float offset_height = (m_window_height - new_height) / 2.0f;

                if (m_window_width >= _aspect_ratio * m_window_height) {
                    _view.setViewport(sf::FloatRect(offset_width / m_window_width, 0.0,
                                                   new_width / m_window_width, 1.0));
                } else {
                    _view.setViewport(sf::FloatRect(0.0, offset_height / m_window_height,
                                                      1.0, new_height / m_window_height));
                }
                break;
        }
    }
}//mate
