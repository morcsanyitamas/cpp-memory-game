#pragma once

class IGame {
public:
    IGame() = default;
    virtual ~IGame() = default;
    virtual void startGame() = 0;
    virtual void endGame() = 0;
protected:
    bool running = false;
};
