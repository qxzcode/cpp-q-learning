#include "State.h"

#include <algorithm> // std::max_element
#include <unordered_map>
#include <fstream>

#include "debug.h"

static constexpr auto NUM_QUANTS = State::NUM_QUANTS;

struct state_key {
    size_t myPaddleY, yourPaddleY;
    size_t ballX, ballY;
    size_t ballVX, ballVY;
    size_t hash;
    
    state_key() {}
    
    explicit state_key(const GameState& gs) {
        myPaddleY = quantize(gs.myPaddleY, NUM_QUANTS);
        yourPaddleY = quantize(gs.yourPaddleY, NUM_QUANTS);
        ballX = quantize(gs.ballX, NUM_QUANTS);
        ballY = quantize(gs.ballY, NUM_QUANTS);
        ballVX = quantize(gs.normBallVX(), NUM_QUANTS);
        ballVY = quantize(gs.normBallVY(), NUM_QUANTS);
    }
    
    static constexpr size_t MAX = 1.0;
    size_t quantize(double prop, size_t numQuants) {
        size_t val = static_cast<size_t>(numQuants * prop/MAX);
        hash = hash*numQuants + val;
        return val;
    }
    
    static double unquantize(size_t val, size_t numQuants) {
        return MAX * (val+0.5)/numQuants;
    }
    
    bool operator==(const state_key& rhs) const {
        return myPaddleY==rhs.myPaddleY && yourPaddleY==rhs.yourPaddleY &&
               ballX==rhs.ballX && ballY==rhs.ballY;
    }
};

struct state_key_hash {
    size_t operator()(const state_key& key) const {
        return key.hash;
    }
};

using stateMap_t = std::unordered_map<state_key, State, state_key_hash>;
stateMap_t states;

State& State::getState(const GameState& gs) {
    state_key key(gs);
    auto it = states.find(key);
    if (it == states.end()) {
        it = states.emplace(key, State(key)).first;
    }
    return it->second;
}

extern int statesFound;

void State::saveStates() {
    std::ofstream out("save.dat", std::ios::binary);
    out << statesFound;
    for (const auto& entry : states) {
        out.write((char*)&entry.first, sizeof(entry.first));
        out.write((char*)&entry.second.Q, sizeof(entry.second.Q));
    }
    cout << "Saved "<<states.size()<<" states" << endl;
    cout << "    "<<statesFound<<"/"<<NUM_STATES<<" states found" << endl;
}

void State::loadStates() {
    std::ifstream in("save.dat", std::ios::binary);
    in >> statesFound;
    while (in.good()) {
        state_key key;
        in.read((char*)&key, sizeof(key));
        State state(key);
        state.found = true;
        in.read((char*)&state.Q, sizeof(state.Q));
        states.emplace(key, std::move(state));
    }
    cout << "Loaded "<<states.size()<<" states" << endl;
    cout << "    "<<statesFound<<"/"<<NUM_STATES<<" states found" << endl;
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