#pragma once
#include "SDL.h"
#include "SDL_timer.h"
#include "../objects/bat.h"

struct skeleton {
public:
    enum AIState {
        IDLE,
        WALK_FORWARD,
        JUMP,
        SHOOT,
    };

    skeleton() {
       this->timer = Timer(); 
       this->state = AIState::WALK_FORWARD;
    }

    void incrementState() {
        if (state == 7) {
            state = 0;
        } else {
            state ++;
        }
    }

    AIState actions[7] = {WALK_FORWARD, IDLE, JUMP, IDLE, WALK_FORWARD, IDLE, SHOOT};
    Timer timer;
    int state;
    float speed = 0.1f;
    float jumpHeight = 5.0f;
};
