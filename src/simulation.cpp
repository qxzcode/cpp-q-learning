#include "simulation.h"

#include <cmath>

std::pair<bool, double> simulation::stepSimulation(GameState& state, double dt) {
    state.ballX += state.ballVX*dt;
    state.ballY += state.ballVY*dt;
    
    if (state.ballY < 0.0) {
        state.ballY = -state.ballY;
        state.ballVY *= -1;
    }
    if (state.ballY > 1.0) {
        state.ballY = 1.0 - state.ballY;
        state.ballVY *= -1;
    }
    
    if (state.ballX < 0.0) {
        double pp = (state.ballY - state.myPaddleY) / PADDLE_RADIUS;
        if (fabs(pp) < 1.0) {
            state.ballX = -state.ballX;
            state.ballVX = -state.ballVX;
            state.ballVY = pp * GameState::MAX_V;
        } else {
            // ball got past my side
            return {true, -1.0};
        }
    }
    
    if (state.ballX > 1.0) {
        double pp = (state.ballY - state.yourPaddleY) / PADDLE_RADIUS;
        if (fabs(pp) < 1.0) {
            state.ballX = 1.0 - state.ballX;
            state.ballVX = -state.ballVX;
            state.ballVY = pp * GameState::MAX_V;
        } else {
            // ball got past your side
            return {true, 1.0};
        }
    }
    
    return {false, -0.04*dt};
}

std::pair<bool, double> simulation::simulateFor(GameState& state, double time) {
    double totalReward = 0;
    for (double t = 0; t < time; t += DELTA_TIME) {
        auto res = stepSimulation(state, DELTA_TIME);
        totalReward += res.second;
        if (res.first) return {true, totalReward};
    }
    return {false, totalReward};
}