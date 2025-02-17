#pragma once
#include "ArmyUnit.h"

class Bomb : public ArmyUnit {
public:
    explicit Bomb(const Players player)
        : ArmyUnit(player, Ranks::Bomb, false) {}
};
