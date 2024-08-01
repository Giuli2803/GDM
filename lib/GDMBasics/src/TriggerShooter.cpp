//
// Created by elly_sparky on 01/02/24.
//

#include "TriggerShooter.h"

namespace mate {
    void TriggerShooter::Loop() {
        if (std::shared_ptr<Element> spt_parent = _parent.lock()) {
            _manager->CheckTrigger(shape, *this);
        }
    }
}
