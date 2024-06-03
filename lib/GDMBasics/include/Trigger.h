//
// Created by elly_sparky on 01/02/24.
//

#ifndef GDMATE_TRIGGER_H
#define GDMATE_TRIGGER_H

#include "GDMBasics.h"

//Todo: TriggerBothWays

namespace mate {
    class Trigger : public Component {
    public:
        mate::Bounds offset = mate::Bounds(nullptr);
        ShapeType shape = RECTANGLE;

        //Todo: More constructors
        explicit Trigger(Element &element) : Component(element){
            offset = mate::Bounds(&element);
            Game::getGame()->AddTrigger(this);
        }
        ~Trigger() override {
            Game::getGame()->RemoveTrigger(this);
        }

        void Loop() override {};
        void RenderLoop() override {};

        virtual void TriggerIn(Element& shooter) = 0;
    };

    class TriggerShooter : public Component {
        TriggerManager *_manager;
    public:
        mate::Bounds offset = mate::Bounds(nullptr);
        ShapeType shape = RECTANGLE;

        //Todo: more constructors
        explicit TriggerShooter(Element* element) : Component(*element){
            _manager = Game::getGame()->getTriggerManager();
            //OMG bitch this is ugly shit
            offset = mate::Bounds(&*element);
        }

        void Loop() override;
        void RenderLoop() override {};
    };
}


#endif //GDMATE_TRIGGER_H
