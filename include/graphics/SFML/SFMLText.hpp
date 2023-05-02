#ifndef IGRAPHICS_SFMLTEXT_HPP
#define IGRAPHICS_SFMLTEXT_HPP

#include "graphics/aText.hpp"
#include <SFML/Graphics.hpp>

namespace igraphicslib {

class Text::Text_ : public sf::Text {
    sf::Font font_;

public:
    Text_(const char* font_filename, const char* text, uint32_t x, uint32_t y);
    Text_(const char* font_filename, const uint32_t* text, uint32_t x, uint32_t y);

    void SetFont(const char* font) {
        font_.loadFromFile(font);
    }

    void SetString(const char* font) {
        this->setString(font);
    }
};

}  // namespace igraphicslib
#endif
