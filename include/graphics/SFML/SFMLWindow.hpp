#ifndef IGRAPHICS_SFMLWINDOW_HPP
#define IGRAPHICS_SFMLWINDOW_HPP
#include <SFML/Graphics.hpp>
#include <cstdint>

namespace IGraphicsLib {

class Window_ : public sf::RenderWindow {
public:
    Window_(uint32_t widht, uint32_t height, const char* title);
    ~Window_() override;
};

}  // namespace IGraphicsLib

#endif
