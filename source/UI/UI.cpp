#include <iostream>
#include <SDL_image.h>
#include <filesystem>
#include <functional>
#include <UI.h>
#include <Texture.h>
#include <unordered_map>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 573

using namespace std;

static auto const PATH = (filesystem::current_path() / "resources").u8string();
static vector<string> const CARDS = {
    "chase_marshall.png",
    "rubble_1.png",
    "rubble_2.png",
    "ryder_1.png",
    "ryder_2.png",
    "sky_1.png",
    "sky_2.png",
    "sky_everest.png"
};

UI::UI(IGame &game) : window(nullptr), renderer(nullptr), game(game) {
    if (!init()) {
        throw std::runtime_error("Failed to initialize UI");
    }

    isDragging = false;
    selectedCardRect = nullptr;

    initLayout();
    loadTextures();

    renderThread = std::thread(&UI::renderLoop, this, 16);  // 60 FPS
}

UI::~UI() {

    //cout << "UI destructor" << endl;

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    IMG_Quit();
    SDL_Quit();
}

void UI::renderLoop(int const delay) const {
    while (renderThreadRunning) {
        render();
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    cout << "Render thread ended" << endl;
}

bool UI::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        return false;
    }
    if (!createWindow()) return false;
    if (!createRenderer()) return false;
    if (!initSDLImage()) return false;
    return true;
}

void UI::initLayout() {
    cardTextures.resize(CARDS.size());
    cardRectangles.resize(CARDS.size());

    backgroundRectangle = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    for(int i = 0; i < CARDS.size(); i++) {
        cardRectangles[i] = {0 + (i * 132), 425, 132, 132};
    }
}

bool UI::loadTextures() {
    backgroundTexture = loadTexture(PATH + "/background.png");
    for(int i = 0; i < CARDS.size(); i++) {
        cardTextures[i] = loadTexture(PATH + "/" + CARDS[i]);
    }
    return true;
}

void UI::render() const {
    SDL_RenderClear(renderer);
    drawBackground();
    drawCards();
    SDL_RenderPresent(renderer);
}

void UI::drawBackground() const {
    backgroundTexture.render(renderer, &backgroundRectangle);
}

void UI::drawCards() const {
    for(int i = 0; i < cardTextures.size(); i++) {
        cardTextures[i].render(renderer, &cardRectangles[i]);
    }
}

Texture UI::loadTexture(const string &filename) const {
    SDL_Texture *imgTexture = IMG_LoadTexture(renderer, filename.c_str());
    if (nullptr == imgTexture) {
        cout << "File not found: " << filename << " SDL_image Error: " << IMG_GetError() << endl;
    }
    return Texture(imgTexture);
}

bool UI::createWindow() {
    window = SDL_CreateWindow("Paw Patrol", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        return false;
    }
    return true;
}

bool UI::createRenderer() {
    if (!window) return false;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        return false;
    }
    return true;
}

bool UI::initSDLImage() {
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
        return false;
    }
    return true;
}

bool UI::isMouseInsideRect(int const mouseX, int const mouseY, SDL_Rect const& rect) {
    return (mouseX > rect.x &&
            mouseX < rect.x + rect.w &&
            mouseY > rect.y &&
            mouseY < rect.y + rect.h);
}

void UI::handleEvents(bool &running) {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {

            renderThreadRunning = false;
            if (renderThread.joinable()) {
                renderThread.join();
            }

            running = false;
            break;
        }

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            handleMouseDownEvent(e, isDragging, originalPosition);
            break;
        }

        if (e.type == SDL_MOUSEMOTION) {
            handleMouseMotionEvent(e, isDragging);
            break;
        }

        if (e.type == SDL_MOUSEBUTTONUP) {
            handleMouseUpEvent(e, originalPosition);
            break;
        }
    }
}

void UI::handleMouseDownEvent(const SDL_Event &e, bool &isDragging,
                              SDL_Point &originalPosition) {
    int const mouseX = e.button.x;
    int const mouseY = e.button.y;

    if (!cardRectangles.empty())
        for (auto &cardRectangle: cardRectangles) {
            if (isMouseInsideRect(mouseX, mouseY, cardRectangle)) {
                isDragging = true;
                selectedCardRect = &cardRectangle;
                originalPosition = {selectedCardRect->x, selectedCardRect->y};
                break;
            }
        }
}

void UI::handleMouseMotionEvent(const SDL_Event &e, const bool &isDragging) {
    int const mouseX = e.motion.x;
    int const mouseY = e.motion.y;

    if (isDragging && selectedCardRect != nullptr) {
        selectedCardRect->x = mouseX - selectedCardRect->w / 2;
        selectedCardRect->y = mouseY - selectedCardRect->h / 2;
    }
}

void UI::handleMouseUpEvent(const SDL_Event &e, SDL_Point &originalPosition) {
    if (selectedCardRect != nullptr) {
        isDragging = false;
        selectedCardRect = nullptr;
        originalPosition = {0, 0};
    }
}


