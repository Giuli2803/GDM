//
// Created by elly_sparky on 23/01/24.
//

#include "Camera.h"

namespace mate{
    Camera::Camera(std::shared_ptr<Element> parent)
    : _parent(std::move(parent))
    {
        _view.setCenter(sf::Vector2f (0, 0));
        _view.setSize(sf::Vector2f (480, 360));
        _target = mate::Game::getGame()->getWindow();
        _target->setView(_view);
        _aspect_ratio = 4.0f/3.0f;
    }

    Camera::~Camera(){
        _target->setView(_target->getDefaultView());
        _target = nullptr;
    }

    void Camera::Loop() {
        if(_target){
            _view.setCenter(_parent->getWorldPosition());
            _view.setRotation(_parent->getWorldRotation());

            //Todo: Update only once? Or redo just in case?
            _target->setView(_view);
        }
    }

    void Camera::WindowResizeEvent() {
        switch (_scale_type) {
            case RESCALE:
                break;
            case REVEAL:
                _view.setSize(_target->getSize().x, _target->getSize().y);
                break;
            case LETTERBOX:
                float m_window_width = _target->getSize().x;
                float m_window_height = _target->getSize().y;
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
