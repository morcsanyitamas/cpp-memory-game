#pragma once
#include <IGame.h>
#include <UI.h>

class Game : public IGame {
    UI ui;
public:
    Game();
    ~Game() = default;
    void startGame() override;
    void endGame() override;
    void run();
};
