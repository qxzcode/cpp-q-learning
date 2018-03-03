#include <fstream>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::cin;

#include "Learner.h"
#include "simulation.h"

GameState gameState;
Learner learner(0.8, 0.9, 0.1);

void reset() {
    
}

void doUpdate() {
    auto res = simulation::simulateFor(gameState, 0.2);
    
}

int main() {
    cout << "== Q-learning ==" << endl;
    
    reset();
    while (true) {
        doUpdate();
    }
    
    return 0;
}