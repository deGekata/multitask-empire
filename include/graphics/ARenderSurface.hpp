#ifndef IGRAPHICS_ARENDERSURFACE_HPP
#define IGRAPHICS_ARENDERSURFACE_HPP

#include "graphics/AText.hpp"
#include "graphics/Color.hpp"
#include "graphics/Surface.hpp"
namespace igraphicslib {
class Texture;

class RenderSurface : public ISurface {
private:
    class RenderSurface_;
    RenderSurface_* rsp_;

public:
    RenderSurface(uint32_t w, uint32_t h);

    ~RenderSurface() override;

    void DrawText(const Text& text) const override;

    void DrawLine(Point p1, Point p2, Color color = colors::kBlack) const override;
    void DrawPoint(Point p, Color color = colors::kBlack) const override;
    void DrawRect(const Rect& rect, Color color = colors::kBlack) const override;
    void DrawHolRect(Point p1, Point p2, Color color = colors::kBlack) const override;
    void DrawSprite(Point pt, const Sprite& sprite) const override;
    void DrawSurf(Point pt, const RenderSurface& surf) const override;
    void DrawSurf(Point pt, const RenderSurface& surf, const Rect& rect) const override;
    void DrawVert(const Vertex* vert, size_t size, PrimitiveType type) const override;
    void DrawTriang(Vertex vert[3]) const override;
    void Update() const override;

    void Clear(Color color = colors::kGray) const override;
    void SetActive(bool active) const override;

    void Resize(uint32_t w, uint32_t h) override;
    igraphicslib::Rect GetSurfRect() const override;

    const Texture& GetTexture() const;

    friend class Window;  // Allow window to access private rsp_;
};

}  // namespace igraphicslib
#endif /* IGRAPHICS_ARENDERSURFACE_HPP */
