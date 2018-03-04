#include "State.h"

#include <algorithm> // std::max_element
#include <unordered_map>
#include <fstream>

#include "debug.h"

struct state_key {
    size_t hash;
    
    state_key() {}
    
    explicit state_key(const GameState& gs) {
        hash = 0;
        for (int i = 0; i < GameState::BUF_SIZE; i++) {
            hash = hash*(GameState::NUM_ITEMS+1) + gs.buf[i];
        }
        hash = hash*(GameState::NUM_ITEMS+1) + gs.heldItem;
        hash = hash*(GameState::BUF_SIZE) + gs.handPos;
    }
    
    bool operator==(const state_key& rhs) const {
        return hash == rhs.hash;
    }
};

struct state_key_hash {
    size_t operator()(const state_key& key) const {
        return key.hash;
    }
};

using stateMap_t = std::unordered_map<state_key, State, state_key_hash>;
stateMap_t states;
extern int statesFound;

State& State::getState(const GameState& gs) {
    state_key key(gs);
    auto it = states.find(key);
    if (it == states.end()) {
        it = states.emplace(key, State(key)).first;
        statesFound++;
    }
    return it->second;
}

void State::saveStates() {
    std::ofstream out("save.dat", std::ios::binary);
    for (const auto& entry : states) {
        out.write((char*)&entry.first, sizeof(entry.first));
        out.write((char*)&entry.second.Q, sizeof(entry.second.Q));
    }
    cout << "Saved "<<states.size()<<"/"<<NUM_STATES<<" states" << endl;
}

void State::loadStates() {
    std::ifstream in("save.dat", std::ios::binary);
    while (in.good()) {
        state_key key;
        in.read((char*)&key, sizeof(key));
        State state(key);
        in.read((char*)&state.Q, sizeof(state.Q));
        states.emplace(key, std::move(state));
    }
    cout << "Loaded "<<states.size()<<"/"<<NUM_STATES<<" states" << endl;
}



double State::getMaxQ() const {
    return *std::max_element(std::begin(Q), std::end(Q));
}

State::State(const state_key& key) {
    // gameState = {
    //     state_key::unquantize(key.myPaddleY, NUM_QUANTS),
    //     state_key::unquantize(key.yourPaddleY, NUM_QUANTS),
    //     state_key::unquantize(key.ballX, NUM_QUANTS),
    //     state_key::unquantize(key.ballY, NUM_QUANTS),
    //     GameState::unnormV(state_key::unquantize(key.ballVX, NUM_QUANTS)),
    //     GameState::unnormV(state_key::unquantize(key.ballVY, NUM_QUANTS))
    // };
    std::fill(std::begin(Q), std::end(Q), INITIAL_Q);
}