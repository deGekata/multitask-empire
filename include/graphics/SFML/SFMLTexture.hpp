#ifndef IGRAPHICS_SFMLTEXTURE_HPP
#define IGRAPHICS_SFMLTEXTURE_HPP
#include "graphics/ATexture.hpp"
#include <SFML/Graphics.hpp>

namespace igraphicslib {

class Texture::Texture_ : public sf::Texture {
public:
    Texture_() = default;
    Texture_(const Texture_& t) = default;
    Texture_& operator=(const Texture_& t) = default;
    ~Texture_() = default;
};
}  // namespace igraphicslib

#endif
