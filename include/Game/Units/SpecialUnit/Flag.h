#pragma once
#include "ArmyUnit.h"

class Flag : public ArmyUnit {
public:
    explicit Flag(const Players player): ArmyUnit(player, Ranks::Flag, false) {
    }
};
