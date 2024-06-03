//
// Created by elly_sparky on 02/01/24.
//

#include "LocalCoords.h"

namespace mate {
    [[maybe_unused]]
    LocalCoords::LocalCoords(){
        _parent = nullptr;
        Transformable::setScale(1, 1);
    }

    LocalCoords::LocalCoords(LocalCoords* parent){
        _parent = parent;
        Transformable::setScale(1, 1);
    }

    [[maybe_unused]]
    LocalCoords::LocalCoords(sf::Vector2f position, LocalCoords *parent = nullptr)
    : LocalCoords(parent) {
        setPosition(position);
    }

    [[maybe_unused]]
    LocalCoords::LocalCoords(sf::Vector2f position, float rotation, LocalCoords *parent = nullptr)
    : LocalCoords(parent) {
        setPosition(position);
        setRotation(rotation);
    }

    [[maybe_unused]]
    LocalCoords::LocalCoords(sf::Vector2f position, sf::Vector2f scale, float rotation, LocalCoords *parent = nullptr)
    : LocalCoords(parent){
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
