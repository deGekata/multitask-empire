#ifndef IGRAPHICS_ATEXT_HPP
#define IGRAPHICS_ATEXT_HPP

#include <geometry/geometry.hpp>
#include <graphics/color.hpp>

namespace igraphicslib {

const char* const kSystemFont = "assets/fonts/OpenSans-Bold.ttf";

using Rect = geometry::Rect2u;
using Point = geometry::Point2i;

class Text {
public:
    enum styleFlags {
        Normal = 0,
        Bold = 1 << 0,
        Italic = 1 << 1,
        Underlined = 1 << 2,
        Strikethrough = 1 << 3,
    };

private:
    class Text_;
    Text_* tp_;

    Rect rect_;
    unsigned char_size_ = 30;
    unsigned style_flags_ = styleFlags::Normal;
    Color color_ = colors::kBlack;
    const char* text_ = nullptr;

    friend class Window;
    friend class RenderSurface;

public:
    Text(const char* font_name, const char* text, uint32_t x = 0, uint32_t y = 0);
    explicit Text(const char* text, uint32_t x = 0, uint32_t y = 0, const char* const font_name = kSystemFont);
    explicit Text(const uint32_t* text, uint32_t x = 0, uint32_t y = 0, const char* const font_name = kSystemFont);
    ~Text();

    void SetPosition(uint32_t x, uint32_t y);
    void SetPosition(const Point& pt);

    void SetColor(const Color& color);
    void SetStyle(uint32_t style);
    void SetCharacterSize(unsigned size);

    void SetFont(const char* font);

    void SetString(const char* string);
    void SetString(const uint32_t* string);

    Rect GetRect() const;
};

}  // namespace igraphicslib
#endif /* IGRAPHICS_ATEXT_HPP */
