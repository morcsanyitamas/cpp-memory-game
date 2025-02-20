#include <Game.h>

using namespace std;

Game::Game() : ui(*this) {}

void Game::run() {
    ui.init();
    ui.loadTextures();
    ui.render();
    //ui.renderBattlefield();
    bool running = true;
    while (running) {
        ui.handleEvents(running);
    }
}
