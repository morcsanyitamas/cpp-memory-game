#pragma once
#include <IGame.h>
#include <SDL.h>
#include <string>
#include <vector>
#include <SDL_UnitRect.h>
#include <Texture.h>

class UI {
public:
    UI(IGame &game);

    ~UI();

    bool init();

    Texture loadTexture(const std::string &filename);

    void getWindowSize(int &width, int &height);

    //SDL_Renderer *getRenderer() { return renderer; }

    //void run(bool &gameEnded, Players &player);

    //void placeUnits(bool &gameEnded, Players &player);

    void delay(int time);

    void renderMenuUI();

    void renderUnitPlacement(const Players &currentPlayer, const std::vector<Ranks> &playerUnits);

    void renderBattlefield();

    void renderMove(const Players &currentPlayer);

    void drawUnit(const SDL_UnitRect *unitRect);

    void drawUnit(const SDL_UnitRect &unitRect);

    void drawUnit(Ranks rank, Players player, int &x, int &y);

    void drawUnit(Ranks rank, Players player, const int &x, const int &y, const int &i);

    void drawStartUnits();

    void drawBattlefieldUnits();

    void drawBattlefieldUnits(const Players &currentPlayer);

    void drawPlayerUI();

    void drawMove();

    void drawHighlight(const SDL_Rect &rect, int thickness = 4);

    void drawStartUnits(const std::vector<Ranks> &playerUnits, const Players &player);

    void drawTexture(Texture &texture, SDL_Rect &rect, int width, int height, int x, int y);

    void handleEvents(bool &running);

    void handleMouseDownEvent(const SDL_Event &e, bool &running, bool &isDragging,
                              SDL_Point &originalPosition);

    void handleMouseUpEvent(const SDL_Event &e, SDL_Point &originalPosition);

    void handleMouseMotionEvent(const SDL_Event &e, const bool &isDragging);

    SDL_Point calculateGridPosition(const SDL_Point &position) const;

    SDL_Point snapToGrid(int mouseX, int mouseY) const;

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    IGame &game;

    bool createWindow();

    bool createRenderer();

    bool initSDLImage();
};
