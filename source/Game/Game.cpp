#include <Game.h>
#include <iostream>

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
    cout << "Game ended" << std::endl;
}
