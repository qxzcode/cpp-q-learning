#pragma once

class GameState {
public:
    static constexpr int BUF_SIZE = 6;
    static constexpr int NUM_ITEMS = BUF_SIZE-1;
    int buf[BUF_SIZE] = {0};
    int heldItem = 0;
    int handPos = 0;
    
    GameState() {
        buf[0] = 1;
    }
    
    int& pointedItem() { return buf[handPos]; }
    
};