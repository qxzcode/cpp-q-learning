#pragma once

#include <utility> // std::pair

#include "Action.h"
#include "GameState.h"

namespace simulation {
    
    std::pair<bool, double> stepSimulation(GameState& state, Action action);
    
}