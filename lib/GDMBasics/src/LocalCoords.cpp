//
// Created by elly_sparky on 02/01/24.
//

#include "LocalCoords.h"
#include <memory>

namespace mate {
    [[maybe_unused]]
    LocalCoords::LocalCoords()
    {
        setScale(1.0f, 1.0f);
    }

    [[maybe_unused]]
    LocalCoords::LocalCoords(sf::Vector2f position, std::shared_ptr<LocalCoords> parent)
    : _parent(std::move(parent)) {
        setPosition(position);
    }

    [[maybe_unused]]
    LocalCoords::LocalCoords(sf::Vector2f position, float rotation, std::shared_ptr<LocalCoords> parent)
    : _parent(std::move(parent)) {
        setPosition(position);
        setRotation(rotation);
    }

    [[maybe_unused]]
    LocalCoords::LocalCoords(sf::Vector2f position, sf::Vector2f scale, float rotation, std::shared_ptr<LocalCoords> parent)
    : _parent(std::move(parent)) {
        setPosition(position);
        setScale(scale);
        setRotation(rotation);
    }

    sf::Vector2f LocalCoords::getWorldPosition() {
        if(_parent)
            return _parent->getWorldPosition() + getPosition();
        else
            return getPosition();
    }

    sf::Vector2f LocalCoords::getWorldScale() {
        sf::Vector2f newScale = getScale();
        if(_parent){
            newScale.x *= _parent->getWorldScale().x;
            newScale.y *= _parent->getWorldScale().y;
        }
        return newScale;
    }

    float LocalCoords::getWorldRotation() const {
        if(_parent)
            return _parent->getWorldRotation() + getRotation();
        else
            return getRotation();
    }
} // mate
