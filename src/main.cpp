#include <thread>
#include <chrono>
#include <cmath>
#include <atomic>

#include "debug.h"
#include "Learner.h"
#include "simulation.h"

GameState gameState;
Learner learner(0.8, 0.96, 0.5);
State* lastState;
Action lastAction;

double random();

double getTime() {
    return std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1000000000.0;
}

void sleep(long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void printState() {
    for (int i = 0; i < GameState::BUF_SIZE; i++) {
        cout << ' ' << gameState.buf[i];
    }
    cout << '\n';
    for (int i = 0; i < GameState::BUF_SIZE; i++) {
        cout << (i == gameState.handPos? " ^" : "  ");
    }
    cout << '\n';
    for (int i = 0; i < GameState::BUF_SIZE; i++) {
        cout << ' ';
        if (i == gameState.handPos) cout << gameState.heldItem;
        else cout << ' ';
    }
    cout << '\n' << endl;
}

int numResets = 0;
void reset() {
    lastState = nullptr;
    gameState = GameState();
    numResets++;
}

int statesFound = 0;
double lastSave = 0;
void doUpdate() {
    lastAction = lastState? learner.chooseAction(*lastState) : Action::NONE;
    auto res = simulation::stepSimulation(gameState, lastAction);
    
    State* curState = res.first? lastState : &State::getState(gameState);
    if (lastState) {
        learner.observeReward(*lastState, lastAction, *curState, res.second);
    }
    lastState = curState;
    
    if (res.first) {
        reset();
        // cout << "reset() "<<res.second << endl;
    }
    
    double time = getTime();
    if (time - lastSave > 60.0) {
        // cout << "Saving states\033[K" << endl;
        State::saveStates();
        lastSave = time;
    }
}


std::atomic<bool> cinReady(false);
void flushCin() {
    cin.seekg(0, std::ios::end);
    cin.clear();
}

int main(int argc, char*[]) {
    cout << "== Q-learning ==" << endl;
    if (argc < 2)
        State::loadStates();
    
    // flush cin
    flushCin();
    
    // start helper thread
    std::thread helperThread([]() {
        cin.peek();
        flushCin();
        cinReady = true;
    });
    
    reset();
    while (!cinReady) {
        doUpdate();
    }
    State::saveStates();
    while (true) {
        doUpdate();
        printState();
    }
    
    helperThread.join();
    return 0;
}