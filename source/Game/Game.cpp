#include <Game.h>

using namespace std;

Game::Game() : ui(*this) {}

void Game::startGame() {
    running = true;
}

void Game::endGame() {
    running = false;
}

void Game::run() {
    startGame();
    while (running) {
        ui.handleEvents(running);
    }
}
