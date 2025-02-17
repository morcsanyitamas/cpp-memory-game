#pragma once
#include <vector>
#include <Field.h>
#include <Obstacle.h>
#include <SDL_rect.h>

class IGame {
public:
    IGame() = default;

    virtual ~IGame() = default;

    virtual void placeUnit(const std::pair<int, int> &to, const Players &player, const Ranks &rank) = 0;

    virtual void removeUnit(const std::pair<int, int> &field) = 0;

    virtual void removeUnit(const SDL_Point &field) = 0;

    virtual bool checkUnitPlaceInBounds(const std::pair<int, int> &to) = 0;

    virtual bool checkMoveInBounds(const std::pair<int, int> &to) const = 0;

    virtual bool checkTargetFieldEmpty(const std::pair<int, int> &to) const = 0;

    virtual void changePlayer(Players &currentPlayer) = 0;

    virtual void middleMirrorBattlefield() = 0;

    virtual bool handleAction(const std::pair<int, int> &from, const std::pair<int, int> &to, Players currentPlayer) = 0;

    virtual void resetGame() = 0;

    virtual bool checkGameOver(Players currentPlayer) = 0;

    std::vector<Players> getPlayers() const { return players; }
    std::vector<Ranks> getPlayerUnits() const { return playerUnits; }
    std::vector<std::vector<std::shared_ptr<Field> > > getBattlefield() const { return battleField; }
    Players getCurrentPlayer() const { return currentPlayer; }
    bool getGameEnded() const { return gameEnded; }
    bool getGameStart() const { return gameStart; }
    bool getUnitPlacement() const { return unitPlacement; }
    bool getBattleStart() const { return battleStart; }
    int getGridSize() const { return gridSize; }

    void setCurrentPlayer(const Players &player) { currentPlayer = player; }
    void setGameEnded(const bool &value) { gameEnded = value; }
    void setGameStart(const bool &value) { gameStart = value; }
    void setUnitPlacement(const bool &value) { unitPlacement = value; }
    void setBattleStart(const bool &value) { battleStart = value; }

protected:
    bool gameEnded = false;
    bool gameStart = false;
    bool running = true;
    bool unitPlacement = false;
    bool battleStart = false;
    const int gridSize = 10;
    Players currentPlayer = Players::Red;
    std::vector<std::vector<std::shared_ptr<Field> > > battleField;
    std::vector<Players> players = {Players::Red, Players::Blue};
    std::vector<Ranks> playerUnits = {
        Ranks::Flag, Ranks::Spy, Ranks::Scout, Ranks::Scout, Ranks::Scout, Ranks::Scout, Ranks::Scout, Ranks::Scout,
        Ranks::Scout, Ranks::Scout, Ranks::Miner, Ranks::Miner,
        Ranks::Miner, Ranks::Miner, Ranks::Miner, Ranks::Sergeant, Ranks::Sergeant, Ranks::Sergeant, Ranks::Sergeant,
        Ranks::Lieutenant, Ranks::Lieutenant,
        Ranks::Lieutenant, Ranks::Lieutenant, Ranks::Captain, Ranks::Captain, Ranks::Captain, Ranks::Captain,
        Ranks::Major, Ranks::Major, Ranks::Major, Ranks::Colonel,
        Ranks::Colonel, Ranks::General, Ranks::Marshal,
        Ranks::Bomb, Ranks::Bomb, Ranks::Bomb, Ranks::Bomb, Ranks::Bomb, Ranks::Bomb
    };
    std::vector<std::pair<int, int> > obstacleLocations = {
        std::pair(4, 2), std::pair(4, 3), std::pair(4, 6), std::pair(4, 7), std::pair(5, 2), std::pair(5, 3),
        std::pair(5, 6), std::pair(5, 7)
    };
};
