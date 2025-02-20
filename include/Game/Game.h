#pragma once
#include <IGame.h>
#include <UI.h>

class Game : public IGame {
    UI ui;
public:
    Game();
    ~Game() = default;
    void run();
};
