#ifndef IGRAPHICS_AWINDOW_HPP
#define IGRAPHICS_AWINDOW_HPP
#include <cstdint>

#include "geometry/geometry.hpp"

#include <graphics/color.hpp>
#include <graphics/event.hpp>
#include <graphics/aObject.hpp>
#include <graphics/aText.hpp>
#include <graphics/aRenderSurface.hpp>
#include <graphics/surface.hpp>
namespace igraphicslib {

using Point = geometry::Point2i;
using Vec2i = geometry::Vector2i;
using Rect = geometry::Rect2<unsigned>;

class WindowPImpl;

class Window : public ISurface {
    WindowPImpl* wp_;

public:
    Window(uint32_t width, uint32_t height, const char* title);
    ~Window() override;

    void Resize(uint32_t width, uint32_t height) override;
    bool PollEvent(Event& event);
    void DrawText(const Text& text) const override;

    void DrawLine(Point p1, Point p2, Color color = colors::kBlack) const override;
    void DrawPoint(Point p, Color color = colors::kBlack) const override;
    void DrawRect(const Rect& rect, Color color = colors::kBlack) const override;
    void DrawHolRect(Point p1, Point p2, Color color = colors::kBlack) const override;
    void DrawSurf(Point pt, const RenderSurface& surf) const override;
    void DrawSurf(Point pt, const RenderSurface& surf, const Rect& rect) const override;
    void DrawVert(const Vertex* vert, size_t size, PrimitiveType type) const override;
    void DrawTriang(Vertex vert[3]) const override;

    void DrawSprite(Point pt, const Sprite& sprite) const override;
    void SetTopmost(bool topmost = true);

    void Clear(Color color = colors::kGray) const override;

    void SetActive(bool active = true) const override;
    igraphicslib::Rect GetSurfRect() const override;

    void Show();
    void Hide();
    void Update() const override;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
};

}  // namespace igraphicslib
#endif /* IGRAPHICS_AWINDOW_HPP */
