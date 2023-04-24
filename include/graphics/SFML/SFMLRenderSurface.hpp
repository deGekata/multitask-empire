#ifndef IGRAPHICS_SFMLRENDERSURFACE_HPP
#define IGRAPHICS_SFMLRENDERSURFACE_HPP
#include <SFML/Graphics.hpp>
#include <cassert>
#include "graphics/ARenderSurface.hpp"
#include "graphics/SFML/SFMLTexture.hpp"

namespace IGraphicsLib {

class RenderSurface::RenderSurface_ : public sf::RenderTexture {
public:
    RenderSurface_(uint32_t w, uint32_t h) {
        bool res = create(w, h);
        texture.tp_ = static_cast<Texture::Texture_*>(const_cast<sf::Texture*>(&getTexture()));
        assert(res);
    }

    ~RenderSurface_() override {
        texture.tp_ = nullptr;  // Prevent deletion of sf::Texture set up in constructor.
    }

    Texture texture{static_cast<Texture::Texture_*>(nullptr)};
};

}  // namespace IGraphicsLib
#endif
