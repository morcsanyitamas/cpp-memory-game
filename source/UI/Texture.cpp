#include "Texture.h"

Texture::Texture(SDL_Texture *texture) : texture(texture) {}
Texture::Texture() : texture(nullptr) {}
Texture::Texture(Texture&& other) noexcept : texture(other.texture) {
    other.texture = nullptr;
}
Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
        texture = other.texture;
        other.texture = nullptr;
    }
    return *this;
}
Texture::~Texture() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}
void Texture::render(SDL_Renderer *renderer, const SDL_Rect *rect) const {
    SDL_RenderCopy(renderer, texture, nullptr, rect);
}