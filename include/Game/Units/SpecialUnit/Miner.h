#pragma once
#include "ArmyUnit.h"

class Miner : public ArmyUnit {
public:
    explicit Miner(const Players player) : ArmyUnit(player, Ranks::Miner, true) {
    }
};
