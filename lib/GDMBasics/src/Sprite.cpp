//
// Created by elly_sparky on 23/01/24.
//
#include "Sprite.h"

namespace mate{
    Sprite::Sprite(Element *element)
    : Component(*element){
        _texture.loadFromFile("../Square.png");
        _sprite.sprite.setTexture(_texture, true);
        _target = mate::Game::getGame()->getWindowTarget();
        _target->printQueue.insert(_target->printQueue.end(), &_sprite);
    }

    Sprite::~Sprite(){
        if(_target)
            _target->printQueue.remove(&_sprite);
    }

    //Todo: Currently _actualize allows to freeze a sprite even when the _element is changing, print in the other hand
    // makes sure the sprite is not added multiple times to the print list. Actualize may be useless, consider.
    // Also make a way to remove the sprite from the printing list.
    void Sprite::Loop() {
        if(_actualize){
            _sprite.sprite.setScale(_element->getWorldScale());
            _sprite.sprite.setRotation(_element->getWorldRotation());
            _sprite.sprite.setPosition(_element->getWorldPosition());
            _sprite.depth = (float) _element->getDepth() + (0.0000000001f * (float) _depth);
        }
    }
}//mate
