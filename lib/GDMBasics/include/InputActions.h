/**
 * @brief InputActions class declaration.
 * @file
 */

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

/**
 * @brief actions related to an specific key.
 * Contains a key and the action (function/method) that gets called when the key is pressed.
 */
struct action_entry
{
    sf::Keyboard::Key key;
    std::function<void()> action;
    std::variant<std::monostate, std::weak_ptr<void>> object_ref;
    ///< If the action is related to an object object_ref allows to check if the object has expired.
};

class InputActions : public Component
{
  private:
    std::list<action_entry> _actions;

  public:
    // Constructors
    explicit InputActions(const std::weak_ptr<Element> &parent) : Component(parent){};

    // Templates
    /**
     * Adds a new input to track, with the action being associated to an object method.
     * @tparam ClassType Class of the object containing the method to call when the action is triggered.
     * @tparam Args Type of the arguments (if any) of the method that's going to be called by the action.
     * @param input sf::Keyboard::Key that triggers the action.
     * @param func Method within the class of the object to call when the action is triggered.
     * @param obj Reference to the specific instance that's going to be used when calling the method of the action.
     * @param args Constant arguments (if any) to send to the method.
     */
    template <typename ClassType, typename... Args>
    void addInput(sf::Keyboard::Key input, void (ClassType::*func)(Args...), std::shared_ptr<ClassType> obj,
                  Args... args)
    {
        action_entry action;
        action.key = input;
        action.object_ref = obj;
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

    /**
     * Adds a new input to track, with the action being associated to a global function.
     * @tparam Args Type of the arguments (if any) of the function that's going to be called by the action.
     * @param input sf::Keyboard::Key that triggers the action.
     * @param func Function to call when the action is triggered.
     * @param args Constant arguments (if any) to send to the function.
     */
    template <typename... Args> void addInput(sf::Keyboard::Key input, void (*func)(Args...), Args... args)
    {
        action_entry action;
        action.key = input;
        action.action = [func, args...]() -> void { return (*func)(args...); };
        action.object_ref = std::monostate{};
        _actions.push_back(action);
    }

    /**
     * loop() implementation will check if any of the keys of the stored action_entry are pressed, if that's the case
     * will execute the corresponding action. If the action is associated to an instance of an object will also check
     * if the instance has expired, in which case the action will be deleted.
     */
    void loop() override;

#ifdef GDM_TESTING_ENABLED
    uint getActionsCount()
    {
        return _actions.size();
    }
#endif
};

inline bool _isKeyPressedFunc(sf::Keyboard::Key key)
{
    return sf::Keyboard::isKeyPressed(key);
}

inline std::function<bool(sf::Keyboard::Key)> isKeyPressed = &_isKeyPressedFunc;

} // namespace mate

#endif // GDMATE_INPUTACTIONS_H
