//
// Created by elly_sparky on 23/01/24.
//
#include "Sprite.h"

namespace mate
{
Sprite::Sprite(const std::weak_ptr<Element> &parent) : Component(parent)
{
    _sprite = std::make_shared<ord_sprite>();
    //_texture.loadFromFile("../Square.png");
    _sprite->sprite.setTexture(_texture, true);
    auto spt_game = Game::getGame();
    _game_manager = spt_game;
}

[[maybe_unused]] void Sprite::addDepth(int depth)
{
    unsigned int stored = _sprite->depth;
    _sprite->depth += depth;
    if (depth < 0 && _sprite->depth > stored)
    {
        _sprite->depth = 0;
    }
    else if (depth > 0 && _sprite->depth < stored)
    {
        _sprite->depth = UINT_MAX;
    }
}

// Todo: Currently _actualize allows to freeze a sprite even when the _element is changing, print in the other hand
//  makes sure the sprite is not added multiple times to the print list. Actualize may be useless, consider.
//  Also make a way to remove the sprite from the printing list.
void Sprite::loop()
{
    if (_actualize)
    {
        if (std::shared_ptr<LocalCoords> spt_parent = _parent.lock())
        {
            _sprite->sprite.setScale(offset.getDimensionBounds(spt_parent->getWorldScale()));
            _sprite->sprite.setRotation(spt_parent->getWorldRotation());
            _sprite->sprite.setPosition(offset.getPositionBounds(spt_parent->getWorldPosition()));
        }
    }
}
} // namespace mate
