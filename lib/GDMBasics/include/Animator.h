//
// Created by elly_sparky on 23/02/24.
//

#ifndef GDMATE_ANIMATOR_H
#define GDMATE_ANIMATOR_H

#include "GDMBasics.h"

namespace mate {
    class Animator {
    private:
        std::list<std::function<void()>> _actions;
        std::shared_ptr<Element> _parent;
    public:
        explicit Animator(std::shared_ptr<Element> parent) : _parent(std::move(parent)){};

        template <typename ClassType, typename... Args>
        void AddFrame(int frame, void (ClassType::*func)(Args...), ClassType& obj, Args... args){
            //Todo: Implement
        }

        template <typename... Args>
        void AddFrame(int frame, void (*func)(Args...), Args... args){
            //Todo: Implement
        }

        //Todo: Implement
        void Loop() {}
        void RenderLoop() {}
        void WindowResizeEvent() {}
    };
} // mate

#endif //GDMATE_ANIMATOR_H
