//
// Created by elly_sparky on 01/08/24.
//

#ifndef GDMATEEXAMPLES_TRIGGERSHOOTER_H
#define GDMATEEXAMPLES_TRIGGERSHOOTER_H

#include "GDMBasics.h"

namespace mate {
    class TriggerShooter: public Component {
    private:
        std::weak_ptr<Game> _game_manager;
        mate::Bounds offset = mate::Bounds();
    public:
        ShapeType shape = RECTANGLE;

        //Todo: more constructors
        explicit TriggerShooter(const std::weak_ptr<Element>& parent) : Component(parent){
            _game_manager = Game::getGame();
            // Todo: Bounds full rework
            offset = mate::Bounds();
        }

        [[nodiscard]] sf::Vector2f getPosition() const {
            if (std::shared_ptr<Element> spt_parent = _parent.lock()){
                return offset.getPositionBounds(spt_parent->getWorldPosition());
            }
            return {0, 0};
        }

        [[nodiscard]] sf::Vector2f getDimensions() const {
            if (std::shared_ptr<Element> spt_parent = _parent.lock()){
                return offset.getDimensionBounds(spt_parent->getWorldScale());
            }
            return {0, 0};
        }

        void setPositionOffset(float left, float top) {
            offset.rect_bounds.left = left;
            offset.rect_bounds.top = top;
        }

        void setDimensionOffset(float width, float height){
            offset.rect_bounds.width = width;
            offset.rect_bounds.height = height;
        }

        void Loop() override;
        // Todo: void ShooterReaction(TriggerType type) = {}; should be called when it activates certain triggers
    };
}

#endif //GDMATEEXAMPLES_TRIGGERSHOOTER_H
