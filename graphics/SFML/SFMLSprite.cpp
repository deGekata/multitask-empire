#include "graphics/SFML/SFMLSprite.hpp"

namespace igraphicslib {

Sprite::Sprite() : sp_(new Sprite_) {
}
Sprite::Sprite(const Texture& texture) : sp_(new Sprite_(texture)) {
}
Sprite::Sprite(const Texture& texture, Rect rect) : sp_(new Sprite_(texture, rect)) {
}

Sprite::~Sprite() {
    delete sp_;
    sp_ = nullptr;
}

void Sprite::SetTexture(const Texture& texture) {
    sp_->setTexture(*texture.tp_);
}

void Sprite::SetTextureRect(const Rect& rect) {
    sp_->setTextureRect(GetSFMLIntRect(rect));
}

Sprite::Sprite(const Sprite& oth, Rect rect) : sp_(new Sprite::Sprite_(*oth.sp_)) {
    sf::IntRect sf_rect = oth.sp_->getTextureRect();
    sp_->setTextureRect({
        sf_rect.left + static_cast<int>(rect.x),
        sf_rect.top + static_cast<int>(rect.y),
        static_cast<int>(rect.w),
        static_cast<int>(rect.h),
    });
}

Sprite::Sprite(const Sprite& oth) : Sprite(oth, oth.GetTextureRect()) {
}

Sprite& Sprite::operator=(const Sprite& oth) {
    *sp_ = *oth.sp_;
    return *this;
}

Rect Sprite::GetTextureRect() const {
    const sf::IntRect& rect = sp_->getTextureRect();
    return Rect{
        static_cast<unsigned>(rect.left),
        static_cast<unsigned>(rect.top),
        static_cast<unsigned>(rect.width),
        static_cast<unsigned>(rect.height),
    };
}

Sprite Sprite::Crop(Rect& rect) {
    return Sprite{*this, rect};
}

void Sprite::Rotate(double angle) {
    sp_->rotate(static_cast<float>(180 * angle / std::numbers::pi));
}

void Sprite::SetRotation(double angle) {
    const sf::IntRect& rect = sp_->getTextureRect();
    sp_->setOrigin(static_cast<float>(rect.width) * 0.5f, static_cast<float>(rect.height) * 0.5f);
    sp_->setRotation(static_cast<float>(180 * angle / std::numbers::pi));
}

void Sprite::SetScale(double scale_x, double scale_y) {
    sp_->setScale(static_cast<float>(scale_x), static_cast<float>(scale_y));
}
}  // namespace igraphicslib
