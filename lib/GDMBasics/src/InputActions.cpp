//
// Created by elly_sparky on 24/01/24.
//

#include "InputActions.h"

namespace mate {
    void InputActions::Loop() {
        for (auto it = _actions.begin(); it != _actions.end();) {
            if (sf::Keyboard::isKeyPressed(it->key)) {
                it->action();
            }

            bool shouldErase = false;
            if (std::holds_alternative<std::weak_ptr<void>>(it->weakRef)) {
                auto weakPtr = std::get<std::weak_ptr<void>>(it->weakRef);
                if (weakPtr.expired()) {
                    shouldErase = true;
                }
            }

            if (shouldErase) {
                it = _actions.erase(it);
            } else {
                ++it;
            }
        }
    }
} //mate
