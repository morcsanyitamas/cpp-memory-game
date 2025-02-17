#pragma once
#include "ArmyUnit.h"

class Spy : public ArmyUnit {
public:
    explicit Spy(const Players player) : ArmyUnit(player, Ranks::Spy, true) {
    }
};
