//
// Created by elly_sparky on 24/01/24.
//

#include "InputActions.h"

namespace mate
{
void InputActions::loop()
{
    for (auto it = _actions.begin(); it != _actions.end();)
    {
        if (std::holds_alternative<std::weak_ptr<void>>(it->object_ref))
        {
            auto weakPtr = std::get<std::weak_ptr<void>>(it->object_ref);
            if (weakPtr.expired())
            {
                it = _actions.erase(it);
                continue;
            }
        }

        if (isKeyPressed(it->key))
        {
            it->action();
        }

        ++it;
    }
}
} // namespace mate
