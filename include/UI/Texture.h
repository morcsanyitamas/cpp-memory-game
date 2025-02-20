#pragma once
#include <memory>
#include <SDL_image.h>

class Texture {
public:
    Texture(SDL_Texture *texture);
    Texture(const Texture& other);
    Texture& operator=(const Texture& other);
    ~Texture();
    void render(SDL_Renderer *renderer, const SDL_Rect *rect) const;
private:
    std::shared_ptr<SDL_Texture> texture;
};
