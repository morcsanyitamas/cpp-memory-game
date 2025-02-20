#pragma once
#include <IGame.h>
#include <SDL.h>
#include <string>
#include <vector>
#include <Texture.h>
#include <thread>
#include <atomic>

class UI {
public:
    explicit UI(IGame &game);
    ~UI();
    void handleEvents(bool &running);
private:
    Texture loadTexture(const std::string &filename) const;

    void handleMouseDownEvent(const SDL_Event &e, bool &isDragging,
                              SDL_Point &originalPosition);

    void handleMouseUpEvent(const SDL_Event &e, SDL_Point &originalPosition);
    void handleMouseMotionEvent(const SDL_Event &e, const bool &isDragging);

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

    std::thread renderThread;
    std::atomic<bool> running;

    bool createWindow();
    bool createRenderer();
    bool initSDLImage();

    bool init();
    void initLayout();

    bool loadTextures();
    void render() const;
    void renderLoop(int delay) const;
    void drawBackground() const;
    void drawCards() const;
    bool isMouseInsideRect(int mouseX, int mouseY, SDL_Rect const& rect);
};
