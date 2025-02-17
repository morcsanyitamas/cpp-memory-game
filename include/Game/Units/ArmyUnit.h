#pragma once
#include "Unit.h"

class ArmyUnit : public Unit {
protected:
    Players player;
    Ranks rank;

public:
    ArmyUnit(Players player, Ranks rank, bool moveable = true)
        : Unit(moveable), player(player), rank(rank) {
    }

    ~ArmyUnit() override = default;

    Players getPlayer() const override { return player; }

    Ranks getRank() const override { return rank; }

    void print() const override {
        std::cout << "[" << getPlayer() << " " << getRank() << "]";
    }
};
