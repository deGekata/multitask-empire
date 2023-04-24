#ifndef IGRAPHICS_ASPRITE_HPP
#define IGRAPHICS_ASPRITE_HPP
#include "geometry/Rect.hpp"

namespace IGraphicsLib {
using Rect = Geometry::Rect2u;
class Texture;
class Sprite {
    class Sprite_;
    Sprite_* sp_;
    friend class RenderSurface;
    friend class Window;

public:
    Sprite();
    Sprite(const Texture& texture);  // FIX: might mark as explicit
    Sprite(const Texture& texture, Rect rect);
    Sprite(const Sprite& oth);
    Sprite(const Sprite& oth, Rect rect);
    Sprite& operator=(const Sprite& oth);

    ~Sprite();

    void setTexture(const Texture& texture);
    void setTextureRect(const Rect& rect);
    Rect getTextureRect() const;
    void rotate(double angle);
    void setRotation(double angle);
    Sprite crop(Rect& rect);

    void setScale(double scaleX, double scaleY);
};
}  // namespace IGraphicsLib

#endif /* IGRAPHICS_ASPRITE_HPP */
