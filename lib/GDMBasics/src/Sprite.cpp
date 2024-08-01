//
// Created by elly_sparky on 23/01/24.
//
#include "Sprite.h"

namespace mate{
    Sprite::Sprite(const std::weak_ptr<Element>& parent)
    : Component(parent){
        _sprite = std::make_shared<ord_sprite>();
        _texture.loadFromFile("../Square.png");
        _sprite->sprite.setTexture(_texture, true);
        _target = mate::Game::getGame()->getWindowTarget();
        _target->printQueue.insert(_target->printQueue.end(), _sprite);
    }

    Sprite::~Sprite(){
        if(_target)
            _target->printQueue.remove(_sprite);
    }

    //Todo: Currently _actualize allows to freeze a sprite even when the _element is changing, print in the other hand
    // makes sure the sprite is not added multiple times to the print list. Actualize may be useless, consider.
    // Also make a way to remove the sprite from the printing list.
    void Sprite::Loop() {
        if(_actualize){
            if (std::shared_ptr<Element> spt_parent = _parent.lock()) {
                _sprite->sprite.setScale(spt_parent->getWorldScale());
                _sprite->sprite.setRotation(spt_parent->getWorldRotation());
                _sprite->sprite.setPosition(spt_parent->getWorldPosition());
                _sprite->depth = (float) spt_parent->getDepth() + (0.0000000001f * (float) _depth);
            }
        }
    }
}//mate
