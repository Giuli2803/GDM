//
// Created by elly_sparky on 13/01/24.
//

#ifndef GDMATE_INPUTACTIONS_H
#define GDMATE_INPUTACTIONS_H

#include "GDMBasics.h"
#include <SFML/System.hpp>
#include <functional>
#include <iostream>
#include <list>
#include <variant>

// Todo:
//  * Add mouse inputs and joystick inputs
//  * Add key combos (Instead of having A does X, I need [A] does X, so I could also do something like [A,B] does Y).

namespace mate
{

struct ActionEntry
{
    sf::Keyboard::Key key;
    std::function<void()> action;
    std::variant<std::monostate, std::weak_ptr<void>> weakRef;
};

class InputActions : public Component
{
  private:
    std::list<ActionEntry> _actions;

  public:
    // Constructors
    explicit InputActions(const std::weak_ptr<Element> &parent) : Component(parent){};

    // Templates
    template <typename ClassType, typename... Args>
    void addInput(sf::Keyboard::Key input, void (ClassType::*func)(Args...), std::shared_ptr<ClassType> obj,
                  Args... args)
    {
        ActionEntry action;
        action.key = input;
        std::weak_ptr<ClassType> w_obj = obj;
        action.action = [w_obj, func, args...]() -> void {
            // Check if the object being referenced still exists
            if (auto sharedObj = w_obj.lock())
            {
                (sharedObj.get()->*func)(args...);
            }
        };
        _actions.push_back(action);
    }

    template <typename... Args> void addInput(sf::Keyboard::Key input, void (*func)(Args...), Args... args)
    {
        ActionEntry action;
        action.key = input;
        action.action = [func, args...]() -> void { return (*func)(args...); };
        action.weakRef = std::monostate{};
        _actions.push_back(action);
    }

    void loop() override;
};
} // namespace mate

#endif // GDMATE_INPUTACTIONS_H
