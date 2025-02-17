#include <algorithm>
#include <Game.h>
#include <iostream>
#include <memory>
#include <ostream>
#include <utility>
#include <vector>
#include <ArmyUnit.h>
#include <Obstacle.h>
#include <Bomb.h>
#include <Scout.h>
#include <Marshal.h>
#include <Miner.h>
#include <Spy.h>
#include <Flag.h>

using namespace std;

Game::Game() : ui(*this) {

}

void Game::run() {
    ui.init();
    ui.renderBattlefield();
    bool running = true;
    while (running) {

        ui.handleEvents(running);

           
    }
}
