#ifndef IGRAPHICS_SURFACE_HPP
#define IGRAPHICS_SURFACE_HPP

#include <graphics/color.hpp>
#include <geometry/geometry.hpp>

namespace igraphicslib {

class Text;
using Point = geometry::Point2i;
using Rect = geometry::Rect2<unsigned>;
class RenderSurface;
class Sprite;

struct Vertex {
    Point point;
    Color color;
};

enum class PrimitiveType { Points, Lines, StripLines, Triangles, StripTriangles, Quads };

class ISurface {
public:
    virtual void DrawText(const Text& text) const = 0;
    virtual void DrawLine(Point p1, Point p2, Color color = colors::kBlack) const = 0;
    virtual void DrawPoint(Point p, Color color = colors::kBlack) const = 0;
    virtual void DrawRect(const Rect& rect, Color color = colors::kBlack) const = 0;
    virtual void DrawHolRect(Point p1, Point p2, Color color = colors::kBlack) const = 0;
    virtual void DrawSprite(Point pt, const Sprite& sprite) const = 0;
    virtual void DrawSurf(Point pt, const RenderSurface& surf) const = 0;
    virtual void DrawSurf(Point pt, const RenderSurface& surf, const Rect& rect) const = 0;
    virtual void DrawTriang(Vertex pt[3]) const = 0;
    virtual void DrawVert(const Vertex* vert, size_t size, PrimitiveType type) const = 0;
    virtual void Clear(Color color = colors::kGray) const = 0;
    virtual void Resize(uint32_t width, uint32_t height) = 0;
    virtual void Update() const = 0;
    virtual void SetActive(bool active = true) const = 0;
    virtual igraphicslib::Rect GetSurfRect() const = 0;

    void DrawHolRect(const Rect& rect, Color color = colors::kBlack) const {
        DrawHolRect(rect.GetCornerLL(), rect.GetCornerGG(), color);
    }
    virtual ~ISurface() = default;
};
}  // namespace igraphicslib
#endif /* IGRAPHICS_SURFACE_HPP */
