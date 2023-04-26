#include "graphics/SFML/SFMLText.hpp"
#include <cstdint>

namespace igraphicslib {
// extern const char* const systemFont = "/home/gms/progs++/vecplot/FreeMono.otf"; //NOLINT

Text::Text_::Text_(const char* font_filename, const char* text, uint32_t x, uint32_t y) {
    if (!font_.loadFromFile(font_filename)) {
        assert(0 && "Font not loaded");
    }
    sf::Text::setFont(font_);
    setString(text);
    setPosition(static_cast<float>(x), static_cast<float>(y));
}

Text::Text_::Text_(const char* font_filename, const uint32_t* text, uint32_t x, uint32_t y) {
    if (!font_.loadFromFile(font_filename)) {
        assert(0 && "Font not loaded");
    }
    sf::Text::setFont(font_);
    setString(text);
    setPosition(static_cast<float>(x), static_cast<float>(y));
}

Text::Text(const char* font_name, const char* text, uint32_t x, uint32_t y) {
    tp_ = new Text_(font_name, text, x, y);
}

Text::Text(const char* text, uint32_t x, uint32_t y, const char* const font_name) {
    tp_ = new Text_(font_name, text, x, y);
}

Text::Text(const uint32_t* text, uint32_t x, uint32_t y, const char* const font_name) {
    tp_ = new Text_(font_name, text, x, y);
}

Text::~Text() {
    delete tp_;
}

void Text::SetPosition(uint32_t x, uint32_t y) {
    tp_->setPosition(static_cast<float>(x), static_cast<float>(y));
}

void Text::SetPosition(const Point& pt) {
    SetPosition(static_cast<uint32_t>(pt.x), static_cast<uint32_t>(pt.y));
}

void Text::SetColor(const Color& color) {
    tp_->setFillColor(sf::Color(color.R(), color.G(), color.B(), color.A()));
}

void Text::SetStyle(unsigned style) {
    tp_->setStyle(style);  // Text styles is equal to SFML
}

void Text::SetCharacterSize(unsigned size) {
    tp_->setCharacterSize(size);
}

void Text::SetFont(const char* font) {
    tp_->SetFont(font);
}

Rect Text::GetRect() const {
    sf::FloatRect rect = tp_->getGlobalBounds();

    return {static_cast<uint32_t>(rect.left), static_cast<uint32_t>(rect.top), static_cast<uint32_t>(rect.width),
            static_cast<uint32_t>(rect.height)};
}

void Text::SetString(const char* string) {
    tp_->SetString(string);
}

void Text::SetString(const uint32_t* string) {
    tp_->setString(string);
}

}  // namespace igraphicslib
