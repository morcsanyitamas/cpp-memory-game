#include <Game.h>

using namespace std;

Game::Game() : ui(*this) {}

void Game::run() {
    bool running = true;
    while (running) {
        ui.handleEvents(running);
    }
}
