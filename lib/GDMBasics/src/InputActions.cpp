//
// Created by elly_sparky on 24/01/24.
//

#include "InputActions.h"

namespace mate {
    void InputActions::Loop() {
        auto actIt = _actions.begin();

        for(sf::Keyboard::Key key : _key_inputs){
            if(sf::Keyboard::isKeyPressed(key)){
                (*actIt)();
            }
            //Todo: safety measures, sizes might not be correct (They should but make sure of that).
            ++actIt;
        }
    }
} //mate