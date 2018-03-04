#include "simulation.h"

#include <algorithm>
#include <iterator>

void sleep(long);

int goal[GameState::BUF_SIZE] = {0,1,2,3,4,5};

std::pair<bool, double> simulation::stepSimulation(GameState& state, Action action) {
    // cout << "Step sim" << endl;
    switch (action) {
        case Action::FWD:
            if (state.handPos == GameState::BUF_SIZE-1) return {true, -1.0};
                state.handPos++;
            break;
        case Action::BACK:
            if (state.handPos == 0) return {true, -1.0};
            state.handPos--;
            break;
        case Action::SWAP:
            if (state.handPos == 0) {
                if (state.heldItem || !state.pointedItem()) return {true, -1.0};
                state.heldItem = state.pointedItem();
                if (state.pointedItem()++ == GameState::NUM_ITEMS) {
                    state.pointedItem() = 0;
                }
            } else {
                std::swap(state.pointedItem(), state.heldItem);
            }
            break;
        case Action::END:
            using std::begin; using std::end;
            if (std::equal(begin(goal), end(goal), begin(state.buf), end(state.buf))) {
                return {true, +1.0};
            } else {
                return {true, -1.0};
            }
        default: break;
    }
    
    return {false, -0.04};
}