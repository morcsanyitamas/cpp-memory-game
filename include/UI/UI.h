#pragma once
#include <IGame.h>
#include <SDL.h>
#include <string>
#include <vector>
#include <Texture.h>

class UI {
public:
    explicit UI(IGame &game);
    ~UI();

    void initLayout();
    bool loadTextures();

    void render();
    void drawBackground();
    void drawCards();

    Texture loadTexture(const std::string &filename);

    void getWindowSize(int &width, int &height);

    //SDL_Renderer *getRenderer() { return renderer; }


    void delay(int time);
    void drawMove();

    void drawHighlight(const SDL_Rect &rect, int thickness = 4);
    void drawTexture(Texture &texture, SDL_Rect &rect, int width, int height, int x, int y);

    void handleEvents(bool &running);
    void handleMouseDownEvent(const SDL_Event &e, bool &running, bool &isDragging,
                              SDL_Point &originalPosition);

    void handleMouseUpEvent(const SDL_Event &e, SDL_Point &originalPosition);
    void handleMouseMotionEvent(const SDL_Event &e, const bool &isDragging);

    SDL_Point calculateGridPosition(const SDL_Point &position) const;
    SDL_Point snapToGrid(int mouseX, int mouseY) const;

private:
    SDL_Rect backgroundRectangle;
    Texture backgroundTexture;

    std::vector<SDL_Rect> cardRectangles;
    std::vector<Texture> cardTextures;

    SDL_Window *window;
    SDL_Renderer *renderer;
    IGame &game;

    bool isDragging;
    SDL_Point originalPosition;
    SDL_Rect *selectedCardRect;

    bool createWindow();
    bool createRenderer();
    bool initSDLImage();
    bool init();
};
