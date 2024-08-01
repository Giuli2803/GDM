//
// Created by elly_sparky on 01/02/24.
//

#ifndef GDMATE_TRIGGER_H
#define GDMATE_TRIGGER_H

#include <utility>

#include "GDMBasics.h"

//Todo: TriggerBothWays

namespace mate {
    class Trigger : public Component {
    public:
        mate::Bounds offset = mate::Bounds();
        ShapeType shape = RECTANGLE;

        //Todo: More constructors
        explicit Trigger(const std::weak_ptr<Element>& parent) : Component(parent){
            // Todo: Full Bounds rework
            offset = mate::Bounds(_parent.lock());
            Game::getGame()->AddTrigger(this);
        }
        ~Trigger() {
            Game::getGame()->RemoveTrigger(this);
        }

        void Loop() override {}
        void RenderLoop() override {}
        void WindowResizeEvent() override {}

        virtual void TriggerIn(Element& shooter) = 0;
    };

    class TriggerShooter: public Component {
    private:
        TriggerManager *_manager;
    public:
        mate::Bounds offset = mate::Bounds();
        ShapeType shape = RECTANGLE;

        //Todo: more constructors
        explicit TriggerShooter(const std::weak_ptr<Element>& parent) : Component(parent){
            _manager = Game::getGame()->getTriggerManager();
            // Todo: Bounds full rework
            offset = mate::Bounds(_parent.lock());
        }

        void Loop() override;
    };
}


#endif //GDMATE_TRIGGER_H
