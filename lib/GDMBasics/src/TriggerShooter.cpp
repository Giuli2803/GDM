//
// Created by elly_sparky on 01/02/24.
//

#include "TriggerShooter.h"

namespace mate {
    void TriggerShooter::Loop() {
        if (std::shared_ptr<Game> spt_game = _game_manager.lock()) {
            spt_game->CheckTrigger(shape, *this);
        }
    }
}
