#pragma once
#include "ArmyUnit.h"

class Marshal : public ArmyUnit {
public:
    explicit Marshal(const Players player): ArmyUnit(player, Ranks::Marshal, true) {
    }
};
