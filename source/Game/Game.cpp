#include <Game.h>

using namespace std;

Game::Game() : ui(*this) {}

void Game::run() {
    ui.init();
    ui.renderBattlefield();
    bool running = true;
    while (running) {
        ui.handleEvents(running);
    }
}
