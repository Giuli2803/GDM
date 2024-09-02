//
// Created by elly_sparky on 23/01/24.
//

#include "Camera.h"
#include "GDMBasics.h"

namespace mate
{
Camera::Camera(const std::weak_ptr<Element> &parent) : Component(parent)
{
    _view.setCenter(sf::Vector2f(0, 0));
    _view.setSize(sf::Vector2f(480, 360));
    _game_manager = mate::Game::getGame();
    _aspect_ratio = 4.0f / 3.0f;
}

Camera::~Camera()
{
    if (auto _spt_game = _game_manager.lock())
    {
        _spt_game->setWindowView();
    }
}

float Camera::getRatio()
{
    auto size = _view.getSize();
    _aspect_ratio = size.x / size.y;
    return _aspect_ratio;
}

unsigned int Camera::useNewTarget(const std::string &title)
{
    if (auto _spt_game = _game_manager.lock())
    {
        target_id = _spt_game->addSecondaryTarget(_view, title);
    }
    return target_id;
}

void Camera::renderLoop()
{
    auto _spt_game = _game_manager.lock();
    if (!_spt_game)
    {
        return;
    }

    if (std::shared_ptr<LocalCoords> spt_parent = _parent.lock())
    {
        _view.setCenter(spt_parent->getWorldPosition());
        _view.setRotation(spt_parent->getWorldRotation());
    }

    _visible_sprites.remove_if([](const std::weak_ptr<const Sprite> &sprite) { return sprite.expired(); });

    _visible_sprites.sort([](const std::weak_ptr<const Sprite> &a, const std::weak_ptr<const Sprite> &b) {
        auto spt_a = a.lock();
        auto spt_b = b.lock();
        int depth_a = spt_a->getElementDepth();
        int depth_b = spt_b->getElementDepth();
        return (depth_a < depth_b || (depth_a == depth_b && spt_a->getSprite()->depth < spt_b->getSprite()->depth));
    });

    for (const auto &sprite : _visible_sprites)
    {
        _spt_game->draw(sprite.lock()->getSprite(), target_id);
    }

    _spt_game->setWindowView(_view, target_id);
}

void Camera::windowResizeEvent()
{
    auto _spt_game = _game_manager.lock();
    if (!_spt_game)
    {
        return;
    }
    switch (_scale_type)
    {
    case RESCALE:
        break;
    case REVEAL:
        _view.setSize((float)_spt_game->getWindowSize(target_id).x, (float)_spt_game->getWindowSize(target_id).y);
        break;
    case LETTERBOX:
        float m_window_width = (float)_spt_game->getWindowSize(target_id).x;
        float m_window_height = (float)_spt_game->getWindowSize(target_id).y;
        float new_width = _aspect_ratio * m_window_height;
        float new_height = m_window_width / _aspect_ratio;
        float offset_width = (m_window_width - new_width) / 2.0f;
        float offset_height = (m_window_height - new_height) / 2.0f;

        if (m_window_width >= _aspect_ratio * m_window_height)
        {
            _view.setViewport(sf::FloatRect(offset_width / m_window_width, 0.0, new_width / m_window_width, 1.0));
        }
        else
        {
            _view.setViewport(sf::FloatRect(0.0, offset_height / m_window_height, 1.0, new_height / m_window_height));
        }
        break;
    }
}
} // namespace mate
