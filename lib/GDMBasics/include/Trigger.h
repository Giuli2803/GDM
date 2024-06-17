//
// Created by elly_sparky on 01/02/24.
//

#ifndef GDMATE_TRIGGER_H
#define GDMATE_TRIGGER_H

#include <utility>

#include "GDMBasics.h"

//Todo: TriggerBothWays

namespace mate {
    class Trigger {
    private:
        std::shared_ptr<Element> _parent;
    public:
        mate::Bounds offset = mate::Bounds();
        ShapeType shape = RECTANGLE;

        //Todo: More constructors
        explicit Trigger(std::shared_ptr<Element> parent) : _parent(std::move(parent)){
            offset = mate::Bounds(_parent);
            Game::getGame()->AddTrigger(this);
        }
        ~Trigger() {
            Game::getGame()->RemoveTrigger(this);
        }

        void Loop() {}
        void RenderLoop() {}
        void WindowResizeEvent() {}

        virtual void TriggerIn(Element& shooter) = 0;
    };

    class TriggerShooter {
    private:
        std::shared_ptr<Element> _parent;
        TriggerManager *_manager;
    public:
        mate::Bounds offset = mate::Bounds();
        ShapeType shape = RECTANGLE;

        //Todo: more constructors
        explicit TriggerShooter(std::shared_ptr<Element> parent) : _parent(std::move(parent)){
            _manager = Game::getGame()->getTriggerManager();
            offset = mate::Bounds(_parent);
        }

        void Loop();
        void RenderLoop() {};
        void WindowResizeEvent() {}
    };
}


#endif //GDMATE_TRIGGER_H
