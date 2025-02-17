#pragma once
#include "Unit.h"

class Obstacle : public Unit {
public:
    Obstacle() : Unit(false) {
    }

    void print() const override {
        std::cout << "[Obsta]";
    }

    Players getPlayer() const override { return Players::None; }

    Ranks getRank() const override { return Ranks::None; }
};
