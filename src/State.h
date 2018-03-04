#pragma once

#include "Action.h"
#include "GameState.h"

struct state_key;

class State {
public:
    static constexpr double INITIAL_Q = 0.0;
    static constexpr long NUM_QUANTS = 4;
    static constexpr long NUM_STATES = NUM_QUANTS*NUM_QUANTS*NUM_QUANTS*NUM_QUANTS*NUM_QUANTS*NUM_QUANTS;
    static State& getState(const GameState& gs);
    static void saveStates();
    static void loadStates();
    
    State(State&& rhs) = default;
    
    double getMaxQ() const;
    
    double Q[Action::NUM_ACTIONS];
    bool found = false;
    
private:
    State(const state_key& key);
    State(const State&) = delete;
    
    // GameState gameState;
    
};