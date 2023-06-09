#include "graphics/SFML/SFMLTexture.hpp"
#include "graphics/SFML/SFMLImage.hpp"

#include <SFML/Graphics.hpp>

namespace igraphicslib {

Texture::Texture() : tp_(new Texture::Texture_) {
}

Texture::Texture(const char* filename) : tp_(new Texture::Texture_) {
    tp_->loadFromFile(filename);
}

Texture::~Texture() {
    delete tp_;
}

bool Texture::LoadFromFile(const char* filename) {
    return tp_->loadFromFile(filename);
}

void Texture::SetRepeated(bool b) const {
    tp_->setRepeated(b);
}

Texture::Texture(const Texture& texture) : tp_(new Texture::Texture_(*texture.tp_)) {
}

Texture& Texture::operator=(const Texture& texture) {
    *tp_ = *texture.tp_;
    return *this;
}

Texture::Texture(const Texture_& tp) : tp_(new Texture::Texture_(tp)) {
}

void Texture::LoadFromImage(const Image& img) {
    tp_->loadFromImage(*(img.ip_));
}

}  // namespace igraphicslib
