#pragma once

#include <utility> // std::pair

#include "Action.h"
#include "GameState.h"

namespace simulation {
    
    constexpr double PADDLE_SIZE = 0.15;
    constexpr double PADDLE_RADIUS = PADDLE_SIZE/2;
    
    constexpr double PADDLE_SPEED = GameState::MAX_V*0.75;
    
    constexpr double DELTA_TIME = 1.0/30;
    
    std::pair<bool, double> stepSimulation(GameState& state, Action action, double dt);
    std::pair<bool, double> simulateFor(GameState& state, Action action, double time);
    
}