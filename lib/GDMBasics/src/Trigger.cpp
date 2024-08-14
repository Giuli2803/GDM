//
// Created by elly_sparky on 14/08/24.
//
#include "GDMBasics.h"

namespace mate
{
sf::Vector2f Trigger::getPosition() const
{
    sf::Vector2f reference(0, 0);
    if (std::shared_ptr<Element> spt_parent = _follow.lock())
    {
        reference = spt_parent->getWorldPosition();
    }
    return _offset.getPositionBounds(reference);
}

sf::Vector2f Trigger::getDimensions() const
{
    sf::Vector2f reference(1, 1);
    if (std::shared_ptr<Element> spt_parent = _follow.lock())
    {
        reference = spt_parent->getWorldScale();
    }
    return _offset.getDimensionBounds(reference);
}

} // namespace mate
