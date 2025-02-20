#include <Texture.h>

Texture::Texture(SDL_Texture *texture): texture(std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture)) {}
Texture::Texture(const Texture& other) : texture(other.texture) {}
Texture::~Texture() = default;

Texture& Texture::operator=(const Texture& other) {
    if (this == &other) return *this;
    texture = other.texture;
    return *this;
}

void Texture::render(SDL_Renderer *renderer, const SDL_Rect *rect) const {
    SDL_RenderCopy(renderer, texture.get(), nullptr, rect);
}
