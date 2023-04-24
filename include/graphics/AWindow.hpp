#ifndef IGRAPHICS_AWINDOW_HPP
#define IGRAPHICS_AWINDOW_HPP
#include <cstdint>
#include "geometry/Geometry.hpp"

#include "graphics/Color.hpp"
#include "graphics/Event.hpp"
#include "graphics/AObject.hpp"
#include "graphics/AText.hpp"
#include "graphics/ARenderSurface.hpp"
#include "graphics/Surface.hpp"
namespace IGraphicsLib {

using Point = Geometry::Point2i;
using Vec2i = Geometry::Vector2i;
using Rect = Geometry::Rect2<unsigned>;

class Window_;

class Window : public Surface {
    Window_* wp_;

public:
    Window(uint32_t width, uint32_t height, const char* title);
    ~Window() override;

    void resize(uint32_t width, uint32_t height) override;
    bool pollEvent(Event& event);
    void drawText(const Text& text) const override;

    void drawLine(Point p1, Point p2, Color color = Colors::Black) const override;
    void drawPoint(Point p, Color color = Colors::Black) const override;
    void drawRect(const Rect& rect, Color color = Colors::Black) const override;
    void drawHolRect(Point p1, Point p2, Color color = Colors::Black) const override;
    void drawSurf(Point pt, const RenderSurface& surf) const override;
    void drawSurf(Point pt, const RenderSurface& surf, const Rect& rect) const override;
    void drawVert(const Vertex* vert, size_t size, PrimitiveType type) const override;
    void drawTriang(Vertex pt[3]) const override;

    void drawSprite(Point pt, const Sprite& sprite) const override;
    void setTopmost(bool topmost = true);

    void clear(Color color = Colors::Gray) const override;

    void setActive(bool active = true) const override;
    IGraphicsLib::Rect getSurfRect() const override;

    void show();
    void hide();
    void update() const override;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
};

}  // namespace IGraphicsLib
#endif /* IGRAPHICS_AWINDOW_HPP */
