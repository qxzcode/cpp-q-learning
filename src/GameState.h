#pragma once

class GameState {
public:
    double myPaddleY, yourPaddleY;
    double ballX, ballY;
    double ballVX, ballVY;
    
    double normBallVX() const { return normV(ballVX); }
    double normBallVY() const { return normV(ballVY); }
    
    static constexpr double MAX_V = 1.0;
    static constexpr double normV(double v) {
        return v/(2*MAX_V) + 0.5;
    }
    static constexpr double unnormV(double norm) {
        return (norm - 0.5) * (2*MAX_V);
    }
    
};