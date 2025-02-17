#pragma once
#include <SDL_rect.h>
#include <string>
#include <utility>

class SDL_UnitRect : public SDL_Rect {
public:
    SDL_UnitRect() = default; // Default constructor
    SDL_UnitRect(Ranks rank, Players player) : SDL_Rect(), rank(rank), player(player) {
    }

    Ranks rank;
    Players player;
};
