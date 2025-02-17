#pragma once
#include "ArmyUnit.h"

class Scout : public ArmyUnit {
public:
    explicit Scout(const Players player) : ArmyUnit(player, Ranks::Scout, true) {
    }
};
