#include <Field.h>
#include <iostream>
#include <memory>
#include <utility>
#include <Unit.h>
#include <ArmyUnit.h>
#include <Obstacle.h>
using namespace std;

void Field::printField() const {
    if (unit) {
        if (auto obs = dynamic_cast<Obstacle *>(unit.get())) {
            obs->print();
        } else if (auto armyUnit = dynamic_cast<ArmyUnit *>(unit.get())) {
            armyUnit->print();
        } else {
            std::cout << "[Unknown Unit]";
        }
    } else {
        std::cout << "[Empty]";
    }
}

void Field::setUnit(const shared_ptr<Unit> &unit) {
    this->unit = unit;
}
