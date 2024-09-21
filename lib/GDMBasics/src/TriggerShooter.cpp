//
// Created by elly_sparky on 01/02/24.
//

#include "TriggerShooter.h"

namespace mate
{
sf::Vector2f TriggerShooter::getPosition() const
{
    if (std::shared_ptr<LocalCoords> spt_parent = _parent.lock())
    {
        return offset.getPositionBounds(spt_parent->getWorldPosition());
    }
    return {0, 0};
}

sf::Vector2f TriggerShooter::getDimensions() const
{
    if (std::shared_ptr<LocalCoords> spt_parent = _parent.lock())
    {
        return offset.getDimensionBounds(spt_parent->getWorldScale());
    }
    return {0, 0};
}

void TriggerShooter::loop()
{
    if (std::shared_ptr<Game> spt_game = _game_manager.lock())
    {
        spt_game->checkTrigger(shape, *this);
    }
}
} // namespace mate
