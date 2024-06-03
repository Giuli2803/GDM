//
// Created by elly_sparky on 23/02/24.
//

#ifndef GDMATE_ANIMATOR_H
#define GDMATE_ANIMATOR_H

#include "GDMBasics.h"

namespace mate {
    class Animator : public Component {
    private:
        std::list<std::function<void()>> _actions;
    public:
        explicit Animator(Element &element) : Component(element){};

        template <typename ClassType, typename... Args>
        void AddFrame(int frame, void (ClassType::*func)(Args...), ClassType& obj, Args... args){
            //Todo: Implement
        }

        template <typename... Args>
        void AddFrame(int frame, void (*func)(Args...), Args... args){
            //Todo: Implement
        }

        //
        void Loop() override;
    };
} // mate

#endif //GDMATE_ANIMATOR_H
