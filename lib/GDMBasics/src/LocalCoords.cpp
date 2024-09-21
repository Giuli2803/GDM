//
// Created by elly_sparky on 02/01/24.
//

#include "LocalCoords.h"
#include <memory>

namespace mate
{
[[maybe_unused]] LocalCoords::LocalCoords()
{
    setScale(1.0f, 1.0f);
}

[[maybe_unused]] LocalCoords::LocalCoords(const std::weak_ptr<LocalCoords> &parent) : _parent(parent)
{
    setScale(1.0f, 1.0f);
}

[[maybe_unused]] LocalCoords::LocalCoords(sf::Vector2f position, const std::shared_ptr<LocalCoords> &parent)
    : _parent(parent)
{
    setPosition(position);
    setScale(1.0f, 1.0f);
}

[[maybe_unused]] LocalCoords::LocalCoords(sf::Vector2f position, float rotation,
                                          const std::shared_ptr<LocalCoords> &parent)
    : _parent(parent)
{
    setPosition(position);
    setScale(1.0f, 1.0f);
    setRotation(rotation);
}

[[maybe_unused]] LocalCoords::LocalCoords(sf::Vector2f position, sf::Vector2f scale,
                                          const std::shared_ptr<LocalCoords> &parent)
    : _parent(parent)
{
    setPosition(position);
    setScale(scale);
}

[[maybe_unused]] LocalCoords::LocalCoords(sf::Vector2f position, sf::Vector2f scale, float rotation,
                                          const std::shared_ptr<LocalCoords> &parent)
    : _parent(parent)
{
    setPosition(position);
    setScale(scale);
    setRotation(rotation);
}

sf::Vector2f LocalCoords::getWorldPosition()
{
    if (auto spt_parent = _parent.lock())
        return spt_parent->getWorldPosition() + getPosition();
    else
        return getPosition();
}

sf::Vector2f LocalCoords::getWorldScale()
{
    sf::Vector2f newScale = getScale();
    if (auto spt_parent = _parent.lock())
    {
        newScale.x *= spt_parent->getWorldScale().x;
        newScale.y *= spt_parent->getWorldScale().y;
    }
    return newScale;
}

float LocalCoords::getWorldRotation() const
{
    if (auto spt_parent = _parent.lock())
        return spt_parent->getWorldRotation() + getRotation();
    else
        return getRotation();
}
} // namespace mate
