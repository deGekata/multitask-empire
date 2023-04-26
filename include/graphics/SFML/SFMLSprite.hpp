#ifndef IGRAPHICS_SFMLSPRITE_HPP
#define IGRAPHICS_SFMLSPRITE_HPP

#include <SFML/Graphics.hpp>
#include "graphics/aSprite.hpp"
#include "graphics/SFML/SFMLTexture.hpp"

namespace igraphicslib {
static inline sf::IntRect GetSFMLIntRect(const Rect& rect) {
    return sf::IntRect{
        static_cast<int>(rect.x),
        static_cast<int>(rect.y),
        static_cast<int>(rect.w),
        static_cast<int>(rect.h),
    };
}

class Sprite::Sprite_ : public sf::Sprite {
public:
    Sprite_() : sf::Sprite() {
    }
    Sprite_(const Sprite_& oth) = default;
    Sprite_& operator=(const Sprite_& oth) = default;
    explicit Sprite_(const Texture& texture) : sf::Sprite(*texture.tp_) {
    }
    Sprite_(const Texture& texture, const Rect& rect) : sf::Sprite(*texture.tp_, GetSFMLIntRect(rect)) {
    }
};
}  // namespace igraphicslib

#endif
