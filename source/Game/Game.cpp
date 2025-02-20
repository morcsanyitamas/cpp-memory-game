#include <Game.h>

using namespace std;

Game::Game() : ui(*this) {}

void Game::run() {
    ui.loadTextures();
    ui.render();
    bool running = true;
    while (running) {
        ui.handleEvents(running);
    }
}
