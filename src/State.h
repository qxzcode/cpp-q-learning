#pragma once

#include "Action.h"
#include "GameState.h"

struct state_key;

class State {
public:
    static constexpr double INITIAL_Q = 0.0;
    static constexpr unsigned NUM_QUANTS = 10;
    static State& getState(const GameState& gs);
    
    State(State&& rhs) = default;
    
    double getMaxQ() const;
    
    double Q[Action::NUM_ACTIONS];
    
private:
    State(const state_key& key);
    State(const State&) = delete;
    
    // GameState gameState;
    
};