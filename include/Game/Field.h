#pragma once
#include <memory>
#include <Unit.h>

class Field {
    std::shared_ptr<Unit> unit = nullptr;

public:
    Field() = default;

    ~Field() = default;

    void setUnit(const std::shared_ptr<Unit> &unit);

    std::shared_ptr<Unit> getUnit() {
        return unit;
    }

    Unit *getUnitPtr() {
        return unit.get();
    }

    void printField() const;
};
