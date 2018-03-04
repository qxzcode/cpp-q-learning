#pragma once

#include "Action.h"
#include "GameState.h"

struct state_key;

using ull_t = unsigned long long;
static constexpr ull_t ipow(ull_t base, int exp, ull_t result = 1) {
    return exp < 1 ? result : ipow(base*base, exp/2, (exp % 2) ? result*base : result);
}

class State {
public:
    static constexpr double INITIAL_Q = 0.0;
    static constexpr ull_t NUM_STATES = ipow(GameState::NUM_ITEMS+1, GameState::BUF_SIZE + 1)*(GameState::BUF_SIZE);
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