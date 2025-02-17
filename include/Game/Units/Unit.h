#pragma once
#include "Enum.h"

class Unit {
protected:
    bool moveable;

public:
    explicit Unit(bool moveable = false) : moveable(moveable) {}

    virtual ~Unit() = default;

    virtual void print() const = 0;

    virtual Players getPlayer() const = 0;

    virtual Ranks getRank() const = 0;

    virtual bool getMoveable() const { return moveable; }
};
