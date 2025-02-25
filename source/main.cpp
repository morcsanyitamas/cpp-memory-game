#include <iostream>
#include <Game.h>

using namespace std;

int main(int, char **) {
    auto game = Game();
    game.run();
    cout << "Exiting..." << endl;
    return 0;
}
