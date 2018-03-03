#pragma once

#include <utility> // std::pair

#include "GameState.h"

namespace simulation {
    
    constexpr double PADDLE_SIZE = 0.1;
    constexpr double PADDLE_RADIUS = PADDLE_SIZE/2;
    
    constexpr double DELTA_TIME = 1.0/30;
    
    std::pair<bool, double> stepSimulation(GameState& state, double dt);
    std::pair<bool, double> simulateFor(GameState& state, double time);
    
}