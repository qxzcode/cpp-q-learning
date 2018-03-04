#pragma once

#include "Action.h"
#include "GameState.h"

struct state_key;

class State {
public:
    static constexpr double INITIAL_Q = 0.0;
    static constexpr long QUANTS_X = 20;
    static constexpr long QUANTS_Y = 40;
    static constexpr long QUANTS_VX = 20;
    static constexpr long QUANTS_VY = 20;
    static constexpr long NUM_STATES = QUANTS_X*QUANTS_Y*QUANTS_Y*QUANTS_Y*QUANTS_VX*QUANTS_VY;
    static State& getState(const GameState& gs);
    static void saveStates();
    static void loadStates();
    
    State(State&& rhs) = default;
    
    double getMaxQ() const;
    
    double Q[Action::NUM_ACTIONS];
    
private:
    State(const state_key& key);
    State(const State&) = delete;
    
    // GameState gameState;
    
};