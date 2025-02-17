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
    initializeBoard();
}

void Game::run() {
    ui.init();
    gameStart = true;
    ui.renderBattlefield(currentPlayer);

    while (!gameEnded) {

        ui.handleEvents(gameEnded, gameStart, unitPlacement, currentPlayer, playerUnits);
           
    }
}

bool Game::checkGameOver(Players currentPlayer) {
    if (gameEnded) return true; // Game already ended
    return !canPlayerMove(currentPlayer == Players::Blue ? Players::Red : Players::Blue);
    // Check if the current player can move
}

bool Game::canPlayerMove(Players player) {
    for (int i = 0; i < battleField.size(); ++i) {
        for (int j = 0; j < battleField[i].size(); ++j) {
            auto unit = battleField[i][j]->getUnitPtr();
            if (unit && unit->getPlayer() == player && unit->getMoveable()) {
                return true;
            }
        }
    }
    return false;
}

void Game::initializeBoard() {
    battleField.resize(10);
    for (int i = 0; i < battleField.size(); i++) {
        battleField[i].resize(10);
        for (int j = 0; j < battleField[i].size(); j++) {
            battleField[i][j] = make_shared<Field>(); // Explicitly initialize each position to nullptr
        }
    }

    for (const auto [x, y]: obstacleLocations) {
        battleField[x][y]->setUnit(make_shared<Obstacle>());
    }
}

void Game::resetGame() {
    initializeBoard();
    currentPlayer = Players::Red;
    gameEnded = false;
    unitPlacement = false;
    gameStart = false;
}

void Game::printBoard() const {
    for (size_t row = 0; row < battleField.size(); ++row) {
        for (size_t col = 0; col < battleField[row].size(); ++col) {
            battleField[row][col]->printField();
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Game::placeUnit(const pair<int, int> &to, const Players &player, const Ranks &rank) {
    const auto unit = makeUnit(player, rank);
    battleField[to.second][to.first]->setUnit(unit);
}

shared_ptr<Unit> Game::makeUnit(const Players &player, const Ranks &rank) {
    switch (rank) {
        case Ranks::Flag:
            return make_shared<Flag>(player);
        case Ranks::Spy:
            return make_shared<Spy>(player);
        case Ranks::Scout:
            return make_shared<Scout>(player);
        case Ranks::Miner:
            return make_shared<Miner>(player);
        case Ranks::Sergeant:
        case Ranks::Lieutenant:
        case Ranks::Captain:
        case Ranks::Major:
        case Ranks::Colonel:
        case Ranks::General:
            return std::make_shared<ArmyUnit>(player, rank);
        case Ranks::Marshal:
            return make_shared<Marshal>(player);
        case Ranks::Bomb:
            return make_shared<Bomb>(player);
        case Ranks::None:
            return make_shared<Obstacle>();
        default:
            nullptr;
    }
}

bool Game::checkUnitPlaceInBounds(const pair<int, int> &to) {
    if (to.first >= 0 && to.first < battleField[0].size() && to.second >= 0 && to.
        second < battleField.size() && checkTargetFieldEmpty(to))
        return true;
    return false;
}

void Game::changePlayer(Players &currentPlayer) {
    currentPlayer = currentPlayer == Players::Red ? Players::Blue : Players::Red;
}

Field *Game::getFieldPtr(const std::pair<int, int> &field) const {
    if (!checkMoveInBounds(field))
        return nullptr;
    return battleField[field.second][field.first].get();
}

void Game::transferUnit(const std::pair<int, int> &from, const std::pair<int, int> &to) {
    getFieldPtr(to)->setUnit(getFieldPtr(from)->getUnit());
    getFieldPtr(from)->setUnit(nullptr);
}

void Game::removeUnit(const std::pair<int, int> &field) {
    getFieldPtr(field)->setUnit(nullptr);
}

void Game::removeUnit(const SDL_Point &field) {
    getFieldPtr({field.x, field.y})->setUnit(nullptr);
}

void Game::middleMirrorBattlefield() {
    const int n = battleField.size();
    reverse(battleField.begin(), battleField.end());

    for (int i = 0; i < n; i++) {
        reverse(battleField[i].begin(), battleField[i].end());
    }
}

bool Game::handleAction(const pair<int, int> &from, const pair<int, int> &to, Players currentPlayer) {
    Unit *unit = getFieldPtr(from)->getUnitPtr();
    if (!unit) {
        cout << "No unit to move from this position." << endl;
        return false;
    }

    // Check move validity
    if (!checkMoveValidation(from, to, currentPlayer)) {
        cout << "Invalid move." << endl;
        return false;
    }

    if (checkTargetFieldEmpty(to)) {
        transferUnit(from, to); // Move to the empty field
        cout << "Moved unit to empty field." << endl;
        return true;
    }
    if (checkTargetFieldHasEnemyUnit(to, unit->getPlayer())) {
        // Only engage in capture if the target is an enemy unit
        handleCapture(from, to);
        return true;
    }
    cout << "Cannot move to a field occupied by an ally unit." << endl;
    return false;
}

void Game::handleCapture(const std::pair<int, int> &from, const std::pair<int, int> &to) {
    Field *fromField = getFieldPtr(from);
    Field *toField = getFieldPtr(to);

    if (!fromField || !toField) {
        cout << "Invalid move: field is out of bounds." << endl;
        return;
    }

    Unit *fromUnit = fromField->getUnitPtr();
    Unit *toUnit = toField->getUnitPtr();

    if (!fromUnit) {
        cout << "No unit at the source field." << endl;
        return;
    }

    // Ensure the target is an enemy unit
    if (!toUnit || fromUnit->getPlayer() == toUnit->getPlayer()) {
        cout << "Cannot capture an allied unit!" << endl;
        return;
    }

    if (toUnit && toUnit->getRank() == Ranks::Flag) {
        gameEnded = true;
        cout << toString(currentPlayer) << " wins by capturing the flag!" << endl;
    }

    if (checkSpecialCaptureRules(fromUnit, toUnit, from, to)) return;

    executeStandardCapture(fromUnit, toUnit, from, to);
}

bool Game::checkSpecialCaptureRules(Unit *fromUnit, Unit *toUnit, const std::pair<int, int> &from,
                                    const std::pair<int, int> &to) {
    if (!fromUnit || !toUnit) {
        cout << "Invalid capture attempt." << endl;
        return false;
    }

    Ranks fromRank = fromUnit->getRank();
    Ranks toRank = toUnit->getRank();

    if ((fromRank == Ranks::Spy && toRank == Ranks::Marshal) ||
        (fromRank == Ranks::Miner && toRank == Ranks::Bomb)) {
        transferUnit(from, to);
        cout << (fromRank == Ranks::Spy ? "Spy captures Marshal!" : "Miner disarms Bomb!") << endl;
        return true;
    }
    return false;
}

void Game::executeStandardCapture(Unit *fromUnit, Unit *toUnit, const std::pair<int, int> &from,
                                  const std::pair<int, int> &to) {
    if (!fromUnit || !toUnit) {
        cout << "Invalid capture logic." << endl;
        return;
    }

    Ranks fromRank = fromUnit->getRank();
    Ranks toRank = toUnit->getRank();

    if (fromRank == toRank) {
        removeUnit(from);
        removeUnit(to);
        cout << "Both units are defeated!" << endl;
    } else if (toRank > fromRank) {
        removeUnit(from);
        cout << "Attacking unit was defeated!" << endl;
    } else {
        transferUnit(from, to);
        cout << "Capture successful by: " << currentPlayer << endl;
    }
}

Ranks Game::checkFieldUnitRank(const std::pair<int, int> &field) const {
    if (const auto unit = getFieldPtr(field)->getUnitPtr()) {
        return unit->getRank();
    }
    return Ranks::None;
}

bool Game::checkMoveInBounds(const std::pair<int, int> &to) const {
    return to.first >= 0 && to.first < gridSize &&
           to.second >= 0 && to.second < gridSize;
}

bool Game::checkUnitMoveable(Unit *unit) const {
    return unit->getMoveable();
}

bool Game::checkIsOwnUnit(const std::pair<int, int> &from, Players currentPlayer) const {
    return getFieldPtr(from)->getUnitPtr() && getFieldPtr(from)->getUnitPtr()->getPlayer() == currentPlayer;
}

bool Game::checkDistanceValidation(const std::pair<int, int> &from, const std::pair<int, int> &to) const {
    if ((abs(from.first - to.first) == 1 && from.second == to.second) ||
        (abs(from.second - to.second) == 1 && from.first == to.first))
        return true;
    return false;
}

bool Game::checkTargetFieldEmpty(const std::pair<int, int> &to) const {
    return getFieldPtr(to)->getUnitPtr() == nullptr;
}

bool Game::checkTargetFieldHasEnemyUnit(const std::pair<int, int> &to, Players player) const {
    auto targetUnit = getFieldPtr(to)->getUnitPtr();
    return targetUnit && (targetUnit->getPlayer() != player);
}

bool Game::checkTargetFieldObstacle(const std::pair<int, int> &to) const {
    auto targetUnit = getFieldPtr(to)->getUnitPtr();
    return targetUnit && targetUnit->getRank() == Ranks::None;
}

bool Game::checkMoveValidation(const std::pair<int, int> &from, const std::pair<int, int> &to,
                               Players currentPlayer) const {
    auto unit = getFieldPtr(from)->getUnitPtr();
    cout << checkMoveInBounds(to) << checkUnitMoveable(unit) << checkDistanceValidation(from, to) <<
            checkIsOwnUnit(from, currentPlayer) << !checkTargetFieldObstacle(to) << endl;
    cout << "Current player: " << toString(currentPlayer) << endl;
    cout << "Unit owned by " << toString(unit->getPlayer()) << endl;
    return unit && checkMoveInBounds(to) && checkUnitMoveable(unit) && checkDistanceValidation(from, to) &&
           checkIsOwnUnit(from, currentPlayer) && !checkTargetFieldObstacle(to);
}
