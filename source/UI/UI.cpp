#include <iostream>
#include <SDL_image.h>
#include <filesystem>
#include <functional>
#include <UI.h>
#include <SDL_UnitRect.h>
#include <Texture.h>
#include <unordered_map>

#define BACKGROUND_WIDTH 1920
#define BACKGROUND_HEIGHT 1080

using namespace std;

static auto path = (filesystem::current_path().parent_path() / ".." / "resources/").u8string();


SDL_Rect backgroundRect;
SDL_Rect quitButtonRect;
SDL_Rect playButtonRect;
SDL_Rect restartButtonRect;
SDL_Rect nextButtonRect;
SDL_Rect logoRect;
vector<SDL_Rect> cardPlacersRect;
vector<SDL_UnitRect> unitPlacerRects;
vector<SDL_UnitRect> unitRects;
bool isDragging = false;
SDL_Point originalPosition;
SDL_UnitRect *selectedPlacerRect = nullptr;
SDL_UnitRect *selectedRect = nullptr;

SDL_Rect *selectedCardRect = nullptr;



UI::UI(IGame &game) : window(nullptr), renderer(nullptr), game(game) {
}

UI::~UI() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
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

void UI::delay(int time) {
    SDL_Delay(time);
}


Texture UI::loadTexture(const string &filename) {
    SDL_Texture *imgTexture = IMG_LoadTexture(renderer, filename.c_str());
    if (nullptr == imgTexture) {
        cout << "File not found: " << filename << " SDL_image Error: " << IMG_GetError() << endl;
        //printf("File not found: %s SDL_image Error: %s\n", filename.c_str(), IMG_GetError());
    }

    return Texture(imgTexture);
}

void UI::getWindowSize(int &width, int &height) {
    SDL_GetWindowSize(window, &width, &height);
}

bool UI::createWindow() {
    window = SDL_CreateWindow("Paw Patrol", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 573,
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
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

void UI::renderUnitPlacement(const Players &currentPlayer, const vector<Ranks> &playerUnits) {
    SDL_SetRenderDrawColor(renderer, 53, 24, 6, 0);
    SDL_RenderClear(renderer);
    drawPlayerUI();

    if (!unitPlacerRects.empty() && unitPlacerRects[0].player == currentPlayer)
        drawStartUnits();
    else
        drawStartUnits(playerUnits, currentPlayer);

    if (currentPlayer == Players::Blue)
        drawBattlefieldUnits(currentPlayer);

    drawMove();

    SDL_RenderPresent(renderer);
}

void UI::drawBattlefieldUnits(const Players &currentPlayer) {
    unitRects.clear();
    unitRects.resize(game.getGridSize() * game.getGridSize());
    auto battleField = game.getBattlefield();
    int x = 0;
    for (int i = 0; i < battleField.size(); i++) {
        for (int j = 0; j < battleField[i].size(); j++) {
            if (battleField[i][j]->getUnit() != nullptr && battleField[i][j]->getUnit()->getRank() != Ranks::None) {
                if (battleField[i][j]->getUnit()->getPlayer() == currentPlayer)
                    drawUnit(battleField[i][j]->getUnit()->getRank(), battleField[i][j]->getUnit()->getPlayer(), j,
                             i, x);
                else
                    drawUnit(Ranks::None, battleField[i][j]->getUnit()->getPlayer(), j,
                             i, x);
                x++;
            }
        }
    }
}

void UI::renderBattlefield(const Players &currentPlayer) {
    SDL_RenderClear(renderer);

    drawPlayerUI();
    //drawBattlefieldUnits();
    //drawMove();

    SDL_RenderPresent(renderer);
}

void UI::drawPlayerUI() {
    Texture background = loadTexture((filesystem::current_path() / "resources" / "background.png").u8string());
    drawTexture(background, backgroundRect, 1024, 573, 0, 0);

    vector<string> cards = {
        "chase_marshall.png", "rubble_1.png", "rubble_2.png", "ryder_1.png", "ryder_2.png", "sky_1.png", "sky_2.png", "sky_everest.png"
    };

    cardPlacersRect.resize(8);

    for(int i = 0; i < cards.size(); i++) {
        Texture cardTexture = loadTexture((filesystem::current_path() / "resources" / cards[i]).u8string());
        drawTexture(cardTexture, cardPlacersRect[i], 132, 132, 0 + (i * 132), 425);

    }

}

void UI::renderMenuUI() {
    SDL_RenderClear(renderer);

    Texture strategoCover = loadTexture(path + "StrategoCover.png");
    SDL_Rect strategoRect;
    drawTexture(strategoCover, strategoRect, 1220, 820, 0, 0);

    Texture playButton = loadTexture(path + "PlayButton.png");
    drawTexture(playButton, playButtonRect, 200, 80, 530, 360);

    Texture quitButton = loadTexture(path + "QuitButton.png");
    drawTexture(quitButton, quitButtonRect, 200, 80, 530, 470);

    SDL_RenderPresent(renderer);
}

void UI::drawUnit(const SDL_UnitRect &unitRect) {
    Texture unitImage = loadTexture(path + "Units/" + toString(unitRect.player) + toString(unitRect.rank) + ".bmp");
    unitImage.render(renderer, &unitRect);
}

void UI::drawUnit(const SDL_UnitRect *unitRect) {
    Texture unitImage = loadTexture(path + "Units/" + toString(unitRect->player) + toString(unitRect->rank) + ".bmp");
    unitImage.render(renderer, unitRect);
}

void UI::drawStartUnits() {
    for (auto &unitRect: unitPlacerRects) {
        Texture unitImage = loadTexture(
            path + "Units/" + toString(unitRect.player) + toString(unitRect.rank) + ".bmp");
        unitImage.render(renderer, &unitRect);
    }
}

void UI::drawBattlefieldUnits() {
    for (auto &rect: unitRects) {
        Texture unitImage = loadTexture(
            path + "Units/" + toString(rect.player) + toString(rect.rank) +
            ".bmp");
        unitImage.render(renderer, &rect);
    }
}

void UI::drawStartUnits(const vector<Ranks> &playerUnits, const Players &player) {
    int xPos = 820;
    int yPos = 180;

    int numUnits = playerUnits.size();
    unitPlacerRects.resize(numUnits);
    int i = 0; // Unit counter

    for (const auto &rank: playerUnits) {
        Texture unitImage = loadTexture(path + "Units/" + toString(player) + toString(rank) + ".bmp");
        // Assuming file format

        SDL_UnitRect rect(rank, player);
        rect.h = 70;
        rect.w = 70;
        rect.x = xPos;
        rect.y = yPos;

        unitPlacerRects[i] = rect;
        unitImage.render(renderer, &rect);

        xPos += 80;

        if ((i + 1) % 5 == 0) {
            xPos = 820;
            yPos += 80;
        }

        i++;
    }
}

void UI::drawUnit(Ranks rank, Players player, int &x, int &y) {
    Texture unitImage = loadTexture(path + "Units/" + toString(player) + toString(rank) + ".bmp");

    SDL_Rect rect;
    rect.h = 70;
    rect.w = 70;
    rect.x = x * 80 + 16;
    rect.y = y * 80 + 16;

    unitImage.render(renderer, &rect);
}

void UI::drawUnit(const Ranks rank, const Players player, const int &x, const int &y, const int &i) {
    Texture unitImage = loadTexture(path + "Units/" + toString(player) + toString(rank) + ".bmp");

    SDL_UnitRect rect(rank, player);
    rect.h = 70;
    rect.w = 70;
    rect.x = x * 80 + 16;
    rect.y = y * 80 + 16;

    unitImage.render(renderer, &rect);
    unitRects[i] = rect;
}

void UI::drawTexture(Texture &texture, SDL_Rect &rect, int width, int height, int x, int y) {
    rect.h = height;
    rect.w = width;
    rect.x = x;
    rect.y = y;
    texture.render(renderer, &rect);
}

bool isMouseInsideRect(int mouseX, int mouseY, SDL_Rect &rect) {
    return (mouseX > rect.x &&
            mouseX < rect.x + rect.w &&
            mouseY > rect.y &&
            mouseY < rect.y + rect.h);
}

void UI::handleEvents(bool &running, bool &unitPlacement,
                      Players &currentPlayer,
                      const vector<Ranks> &playerUnits) {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            running = false;
            break;
        }

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            handleMouseDownEvent(e, running, unitPlacement, currentPlayer,
                                 playerUnits, isDragging, originalPosition);
            break;
        }

        if (e.type == SDL_MOUSEMOTION) {
            handleMouseMotionEvent(e, isDragging);
            break;
        }

        if (e.type == SDL_MOUSEBUTTONUP) {
            handleMouseUpEvent(e, originalPosition, currentPlayer);
            break;
        }
    }
}

void UI::handleMouseDownEvent(const SDL_Event &e, bool &running, bool &unitPlacement,
                              Players &currentPlayer, const vector<Ranks> &playerUnits, bool &isDragging,
                              SDL_Point &originalPosition) {
    int mouseX = e.button.x;
    int mouseY = e.button.y;




    if (!cardPlacersRect.empty())
        for (auto &cardPlacerRect: cardPlacersRect) {
            if (isMouseInsideRect(mouseX, mouseY, cardPlacerRect)) {
                isDragging = true;
                selectedCardRect = &cardPlacerRect;
                originalPosition = {selectedCardRect->x, selectedCardRect->y};
                break;
            }
        }
/*
    if (!unitRects.empty())
        for (auto &unitRect: unitRects) {
            if (isMouseInsideRect(mouseX, mouseY, unitRect)) {
                isDragging = true;
                selectedRect = &unitRect;
                originalPosition = {selectedRect->x, selectedRect->y};
                break;
            }
        }
        */
}

void UI::handleMouseMotionEvent(const SDL_Event &e, const bool &isDragging) {
    int mouseX = e.motion.x;
    int mouseY = e.motion.y;

    if (isDragging && selectedCardRect != nullptr) {
        selectedCardRect->x = mouseX - selectedCardRect->w / 2;
        selectedCardRect->y = mouseY - selectedCardRect->h / 2;

        // Frissítsd a képernyőt
        SDL_RenderClear(renderer);

        Texture background = loadTexture((filesystem::current_path() / "resources" / "background.png").u8string());
        drawTexture(background, backgroundRect, 1024, 573, 0, 0);

        vector<string> cards = {
            "chase_marshall.png", "rubble_1.png", "rubble_2.png", "ryder_1.png", "ryder_2.png", "sky_1.png", "sky_2.png", "sky_everest.png"
        };

        for(int i = 0; i < cards.size(); i++) {
            Texture cardTexture = loadTexture((filesystem::current_path() / "resources" / cards[i]).u8string());
            cardTexture.render(renderer, &cardPlacersRect[i]);

        }

        SDL_RenderPresent(renderer);
    }



    /*
    if (isDragging && selectedRect != nullptr) {
        selectedRect->x = mouseX - selectedRect->w / 2;
        selectedRect->y = mouseY - selectedRect->h / 2;
    }
    */
}

void UI::handleMouseUpEvent(const SDL_Event &e, SDL_Point &originalPosition,
                            Players &currentPlayer) {
    int mouseX = e.button.x;
    int mouseY = e.button.y;
    int flooredX = 0;
    int flooredY = 0;

    auto snappedPosition = snapToGrid(mouseX, mouseY);

    if (!game.getUnitPlacement()) {
        if (currentPlayer == Players::Blue) {
            flooredX = (snappedPosition.x + 10) / 80;
            flooredY = snappedPosition.y - 490 < 0 ? -1 : (snappedPosition.y + 10) / 80;
        } else {
            flooredX = backgroundRect.w / 80 - 1 - (snappedPosition.x + 10) / 80;
            flooredY = snappedPosition.y - 490 < 0 ? -1 : backgroundRect.h / 80 - 1 - (snappedPosition.y + 10) / 80;
        }
    } else {
        flooredX = (snappedPosition.x + 10) / 80;
        flooredY = snappedPosition.y < 0 ? -1 : (snappedPosition.y + 10) / 80;
    }

    if (selectedCardRect != nullptr) {
        //cout << game.checkUnitPlaceInBounds({flooredX, flooredY}) << endl;
        if (true) {
            selectedCardRect->x = snappedPosition.x;
            selectedCardRect->y = snappedPosition.y;
            auto pos = calculateGridPosition(originalPosition, currentPlayer);
            /*if (game.checkMoveInBounds({pos.x, pos.y}) && !game.checkTargetFieldEmpty({pos.x, pos.y})) {
                game.removeUnit(pos);
            }
            game.placeUnit({flooredX, flooredY}, selectedPlacerRect->player, selectedPlacerRect->rank);*/
        }
        isDragging = false;
        selectedCardRect = nullptr;
        originalPosition = {0, 0};
    }

    if (selectedRect != nullptr) {
        auto from = calculateGridPosition(originalPosition, currentPlayer);
        cout << "FROM X: " << from.x << " FROM Y: " << from.y << endl;
        auto to = calculateGridPosition(snappedPosition, currentPlayer);
        cout << "TO X: " << flooredX << " TO Y: " << flooredY << endl;
        if (game.handleAction({from.x, from.y}, {flooredX, flooredY},
                              currentPlayer)) {
            selectedRect->x = snappedPosition.x;
            selectedRect->y = snappedPosition.y;
            if (game.checkGameOver(currentPlayer))
                game.setGameEnded(true);
            game.changePlayer(currentPlayer);
            game.middleMirrorBattlefield();
            SDL_Delay(300);
            drawBattlefieldUnits(currentPlayer);
        } else {
            selectedRect->x = originalPosition.x;
            selectedRect->y = originalPosition.y;
        }
        isDragging = false;
        selectedRect = nullptr;
        originalPosition = {0, 0};
    }
}

SDL_Point UI::calculateGridPosition(const SDL_Point &position, const Players &currentPlayer) const {
    if (!game.getUnitPlacement())
        return {
            backgroundRect.w / 80 - 1 - (position.x + 10) / 80,
            position.y - 490 < 0 ? -1 : backgroundRect.h / 80 - 1 - (position.y + 10) / 80
        };
    return {
        (position.x / (backgroundRect.w / 10) * (backgroundRect.w / 10) + 16) / 80,
        (position.y / (backgroundRect.h / 10) * (backgroundRect.h / 10) + 16) / 80
    };
}


SDL_Point UI::snapToGrid(const int mouseX, const int mouseY) const {
    const int snapX = mouseX / (backgroundRect.w / game.getGridSize()) * (backgroundRect.w / game.getGridSize()) +
                      16;
    const int snapY = mouseY / (backgroundRect.h / game.getGridSize()) * (backgroundRect.h / game.getGridSize()) +
                      16;
    return {snapX, snapY};
}

void UI::drawMove() {
    if (isDragging && selectedPlacerRect != nullptr) {
        drawUnit(selectedPlacerRect);
        drawHighlight(*selectedPlacerRect);
    }

    if (isDragging && selectedRect != nullptr) {
        drawUnit(selectedRect);
        drawHighlight(*selectedRect);
    }
}

void UI::drawHighlight(const SDL_Rect &rect, const int thickness) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (int offset = 0; offset < thickness; offset++) {
        SDL_Rect highlightRect = {rect.x - offset, rect.y - offset, rect.w + 2 * offset, rect.h + 2 * offset};
        SDL_RenderDrawRect(renderer, &highlightRect);
    }
}
