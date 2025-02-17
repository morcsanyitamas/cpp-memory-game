#pragma once
#include "ArmyUnit.h"

class NonSpecialUnit : public ArmyUnit {
public:
    NonSpecialUnit(Players player, Ranks rank) : ArmyUnit(player, rank, true) {
    }
};
