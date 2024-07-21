//
// Created by elly_sparky on 13/01/24.
//

#ifndef GDMATE_INPUTACTIONS_H
#define GDMATE_INPUTACTIONS_H

#include <iostream>
#include <list>
#include <SFML/System.hpp>
#include <functional>
#include <variant>
#include "GDMBasics.h"

//Todo:
// * In this way each _element that reads inputs must check them, with possible repetitions.
// ** Solution 1: Change the way inputs are read
// ** Solution 2: The user could make am "InputReaderElement" that notifies all required objects.
//  The library could have an example of this. The problem is that one input should call multiple functions.
//  *** Solution 2.1: Add am object that stores functions an has a public method to call them all, then call that method.
//  *** Solution 2.2: Change the way actions are stored.
// * Add safety measures. Check if the instance of the object still exist before calling any method.
// * Add mouse inputs and joystick inputs
// * Add key combos (Instead of having A does X, I need [A] does X, so I could also do something like [A,B] does Y).

namespace mate{

    struct ActionEntry {
        sf::Keyboard::Key key;
        std::function<void()> action;
        std::variant<std::monostate, std::weak_ptr<void>> weakRef;
    };

    class InputActions {
    private:
        std::shared_ptr<Element> _parent;
        std::list<ActionEntry> _actions;
    public:
        //Constructors
        explicit InputActions(std::shared_ptr<Element> parent) : _parent(std::move(parent)){};

        template <typename ClassType, typename... Args>
        void AddInput(sf::Keyboard::Key input, void (ClassType::*func)(Args...), std::shared_ptr<ClassType> obj, Args... args) {
            ActionEntry action;
            action.key = input;
            std::weak_ptr<ClassType> w_obj = obj;
            action.action = [w_obj, func, args...]() -> void {
                // Check if the object being referenced still exists
                if (auto sharedObj = w_obj.lock()) {
                    (sharedObj.get()->*func)(args...);
                }
            };
            _actions.push_back(action);
        }

        template <typename... Args>
        void AddInput(sf::Keyboard::Key input, void (*func)(Args...), Args... args) {
            ActionEntry action;
            action.key = input;
            action.action = [func, args...]()->void {
                return (*func)(args...);
            };
            action.weakRef = std::monostate{};
            _actions.push_back(action);
        }

        void Loop();
        void RenderLoop() {}
        void WindowResizeEvent() {}
    };
}//mate

#endif //GDMATE_INPUTACTIONS_H
