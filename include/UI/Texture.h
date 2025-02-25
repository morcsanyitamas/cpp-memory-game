#pragma once
#include <SDL_image.h>

class Texture {
public:
    explicit Texture(SDL_Texture *texture);
    Texture();
    Texture(const Texture& other) = delete;
    Texture& operator=(const Texture& other) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;
    ~Texture();
    void render(SDL_Renderer *renderer, const SDL_Rect *rect) const;
private:
    SDL_Texture *texture;
};