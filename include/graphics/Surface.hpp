#ifndef IGRAPHICS_SURFACE_HPP
#define IGRAPHICS_SURFACE_HPP
#include "graphics/Color.hpp"
#include "geometry/Geometry.hpp"

namespace IGraphicsLib {
class Text;
using Point = Geometry::Point2i;
using Rect = Geometry::Rect2<unsigned>;
class RenderSurface;
class Sprite;

struct Vertex {
    Point point;
    Color color;
};

enum class PrimitiveType { Points, Lines, StripLines, Triangles, StripTriangles, Quads };

class Surface {
public:
    virtual void drawText(const Text& text) const = 0;
    virtual void drawLine(Point p1, Point p2, Color color = Colors::Black) const = 0;
    virtual void drawPoint(Point p, Color color = Colors::Black) const = 0;
    virtual void drawRect(const Rect& rect, Color color = Colors::Black) const = 0;
    virtual void drawHolRect(Point p1, Point p2, Color color = Colors::Black) const = 0;
    virtual void drawSprite(Point pt, const Sprite& sprite) const = 0;
    virtual void drawSurf(Point pt, const RenderSurface& surf) const = 0;
    virtual void drawSurf(Point pt, const RenderSurface& surf, const Rect& rect) const = 0;
    virtual void drawTriang(Vertex pt[3]) const = 0;
    virtual void drawVert(const Vertex* vert, size_t size, PrimitiveType type) const = 0;
    virtual void clear(Color color = Colors::Gray) const = 0;
    virtual void resize(uint32_t width, uint32_t height) = 0;
    virtual void update() const = 0;
    virtual void setActive(bool active = true) const = 0;
    virtual IGraphicsLib::Rect getSurfRect() const = 0;

    void drawHolRect(const Rect& rect, Color color = Colors::Black) const {
        drawHolRect(rect.getCornerLL(), rect.getCornerGG(), color);
    }
    virtual ~Surface() {
    }
};
}  // namespace IGraphicsLib
#endif /* IGRAPHICS_SURFACE_HPP */
