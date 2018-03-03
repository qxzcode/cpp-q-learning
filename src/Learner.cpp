#include "Learner.h"

#include <limits>
#include <random>
#include <chrono>

static std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
static std::uniform_real_distribution<double> dist(0.0, 1.0);
static double random() {
    return dist(gen);
}

void Learner::observeReward(State& curState, Action action, State& nextState, double reward) {
    double newQ = reward + discountFactor*nextState.getMaxQ();
    curState.Q[action] = (1-learningRate)*curState.Q[action] + (learningRate)*newQ;
}

Action Learner::chooseAction(const State& state) const {
    Action bestAction = Action::NUM_ACTIONS;
    double bestQ = -std::numeric_limits<double>::max();
    for (int ai = 0; ai < Action::NUM_ACTIONS; ai++) {
        double Q = state.Q[ai] + random()*qRandomness;
        if (Q > bestQ) {
            bestQ = Q;
            bestAction = static_cast<Action>(ai);
        }
    }
    return bestAction;
}