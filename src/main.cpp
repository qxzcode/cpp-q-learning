#include <thread>
#include <chrono>
#include <cmath>
#include <atomic>

#include "debug.h"
#include "Learner.h"
#include "simulation.h"

GameState gameState;
Learner learner(0.8, 0.96, 0.1);
State* lastState;
Action lastAction;

double random();

double getTime() {
    return std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1000000000.0;
}

void sleep(long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void setCursorPos(int x, int y) {
    cout << "\033["<<y<<";"<<x<<"H";
}

void printState() {
    sleep(50);
    
    constexpr int WIDTH = 60;
    constexpr int HEIGHT = 20;
    cout << "\033c"; // clear console
    
    setCursorPos(gameState.ballX*WIDTH+1, (1-gameState.ballY)*HEIGHT+1);
    cout << "O";
    for (int y = 0; y < HEIGHT; y++) {
        double yf = 1 - (y + 0.5)/HEIGHT;
        if (fabs(yf - gameState.myPaddleY) < simulation::PADDLE_RADIUS) {
            setCursorPos(1, y+1);
            cout << "|";
        }
        if (fabs(yf - gameState.yourPaddleY) < simulation::PADDLE_RADIUS) {
            setCursorPos(WIDTH, y+1);
            cout << "|";
        }
    }
    
    setCursorPos(0, HEIGHT+1);
    cout << std::flush;
}

int numResets = 0;
void reset() {
    lastState = nullptr;
    gameState.myPaddleY = 0.5;
    gameState.yourPaddleY = 0.5;
    gameState.ballX = 0.5;
    gameState.ballY = random();
    gameState.ballVX = GameState::MAX_V*0.75*-1;
    gameState.ballVY = GameState::MAX_V*0.5*(random()<0.5? 1 : -1);
    numResets++;
}

int statesFound = 0;
double lastSave = 0;
void doUpdate() {
    auto res = simulation::simulateFor(gameState,
                                       lastState? learner.chooseAction(*lastState) : Action::NONE,
                                       0.05);
    
    State* curState = res.first? lastState : &State::getState(gameState);
    if (curState && !curState->found) {
        curState->found = true;
        statesFound++;
        // cout << "States found: "<<statesFound<<" / "<<State::NUM_STATES<<"\r" << std::flush;
    }
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

int main() {
    cout << "== Q-learning ==" << endl;
    // State::loadStates();
    
    // flush cin
    cin.seekg(0, std::ios::end);
    cin.clear();
    
    // start helper thread
    std::thread helperThread([]() {
        cin.peek();
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