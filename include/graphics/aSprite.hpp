#ifndef IGRAPHICS_ASPRITE_HPP
#define IGRAPHICS_ASPRITE_HPP
#include <geometry/rect.hpp>

namespace igraphicslib {

using Rect = geometry::Rect2u;
class Texture;
class Sprite {
    class Sprite_;
    Sprite_* sp_;
    friend class RenderSurface;
    friend class Window;

public:
    Sprite();
    
    //NOLINTNEXTLINE
    Sprite(const Texture& texture);  // FIX: might mark as explicit
    Sprite(const Texture& texture, Rect rect);
    Sprite(const Sprite& oth);
    Sprite(const Sprite& oth, Rect rect);
    Sprite& operator=(const Sprite& oth);

    ~Sprite();

    void SetTexture(const Texture& texture);
    void SetTextureRect(const Rect& rect);
    Rect GetTextureRect() const;
    void Rotate(double angle);
    void SetRotation(double angle);
    Sprite Crop(Rect& rect);

    void SetScale(double scale_x, double scale_y);
};
}  // namespace igraphicslib

#endif /* IGRAPHICS_ASPRITE_HPP */
