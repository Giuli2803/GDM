//
// Created by elly_sparky on 01/02/24.
//

#ifndef GDMATE_TRIGGER_H
#define GDMATE_TRIGGER_H

#include <utility>

#include "GDMBasics.h"

//Todo: TriggerBothWays

namespace mate {
    // Igual Trigger podría ser una cosa totalmente aparte sin heredar de Component e incluso sin un Element padre
    class Trigger : public Component {
    private:
        mate::Bounds offset = mate::Bounds();
    public:
        ShapeType shape = RECTANGLE;

        //Todo: More constructors
        explicit Trigger(const std::weak_ptr<Element>& parent) : Component(parent){
            // Todo: Full Bounds rework
            offset = mate::Bounds();
            Game::getGame()->AddTrigger(this);
        }
        ~Trigger() {
            Game::getGame()->RemoveTrigger(this);
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

        void setDimentionOffset(float width, float height){
            offset.rect_bounds.width = width;
            offset.rect_bounds.height = height;
        }

        void Loop() override {}
        void RenderLoop() override {}
        void WindowResizeEvent() override {}

        virtual void TriggerIn() = 0;
    };

    class TriggerShooter: public Component {
    private:
        TriggerManager *_manager;
        mate::Bounds offset = mate::Bounds();
    public:
        ShapeType shape = RECTANGLE;

        //Todo: more constructors
        explicit TriggerShooter(const std::weak_ptr<Element>& parent) : Component(parent){
            _manager = Game::getGame()->getTriggerManager();
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

        void setDimentionOffset(float width, float height){
            offset.rect_bounds.width = width;
            offset.rect_bounds.height = height;
        }

        void Loop() override;
        // Todo: void ShooterReaction(TriggerType type) = {}; should be called when it activates certain triggers
    };
}


#endif //GDMATE_TRIGGER_H
