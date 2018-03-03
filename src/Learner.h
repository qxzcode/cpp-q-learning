#pragma once

#include "State.h"
#include "Action.h"

class Learner {
public:
    Learner(double learningRate, double discountFactor, double qRandomness):
        learningRate(learningRate),discountFactor(discountFactor),qRandomness(qRandomness) {}
    
    void observeReward(State& curState, Action action, State& nextState, double reward);
    
    Action chooseAction(const State& state) const;
    
    double learningRate, discountFactor, qRandomness;
    
};