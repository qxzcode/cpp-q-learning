#include "simulation.h"

#include <cmath>

#include "debug.h"
void sleep(long);
void printState();

static void clamp(double& val, double min, double max) {
    if (val < min) val = min;
    if (val > max) val = max;
}

static void average(double& val, double other) {
    val = (val + other) / 2;
}

std::pair<bool, double> simulation::stepSimulation(GameState& state, Action action, double dt) {
    // cout << "Step sim" << endl;
    // update paddle positions
    switch (action) {
    case Action::UP:
        state.myPaddleY += PADDLE_SPEED*dt;
        break;
    case Action::DOWN:
        state.myPaddleY -= PADDLE_SPEED*dt;
        break;
    default:
        break;
    }
    clamp(state.myPaddleY, 0, 1);
    
    state.yourPaddleY += PADDLE_SPEED*0.5*dt * (state.yourPaddleY < state.ballY? 1 : -1);
    clamp(state.yourPaddleY, 0, 1);
    
    // update ball & check for collisions
    state.ballX += state.ballVX*dt;
    state.ballY += state.ballVY*dt;
    
    if (state.ballY < 0.0) {
        state.ballY = -state.ballY;
        state.ballVY *= -1;
    }
    if (state.ballY > 1.0) {
        state.ballY = 2.0 - state.ballY;
        state.ballVY *= -1;
    }
    
    if (state.ballX < 0.0) {
        double pp = (state.ballY - state.myPaddleY) / PADDLE_RADIUS;
        if (fabs(pp) < 1.0) {
            state.ballX = -state.ballX;
            state.ballVX = -state.ballVX;
            average(state.ballVY, pp * GameState::MAX_V);
        } else {
            // ball got past my side
            return {true, -1.0};
        }
    }
    
    if (state.ballX > 1.0) {
        double pp = (state.ballY - state.yourPaddleY) / PADDLE_RADIUS;
        if (fabs(pp) < 1.0) {
            state.ballX = 2.0 - state.ballX;
            state.ballVX = -state.ballVX;
            average(state.ballVY, pp * GameState::MAX_V);
        } else {
            // ball got past your side
            return {true, 10.0};
        }
    }
    
    return {false, -0.1*dt};
}

std::pair<bool, double> simulation::simulateFor(GameState& state, Action action, double time) {
    double totalReward = 0;
    for (double t = 0; t < time; t += DELTA_TIME) {
        auto res = stepSimulation(state, action, DELTA_TIME);
        totalReward += res.second;
        if (res.first) {
            // cout << "ended" << endl;
            return {true, totalReward};
        }
    }
    // cout << "didn't end" << endl;
    return {false, totalReward};
}