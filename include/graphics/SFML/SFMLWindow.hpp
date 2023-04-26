#ifndef IGRAPHICS_SFMLWINDOW_HPP
#define IGRAPHICS_SFMLWINDOW_HPP
#include <SFML/Graphics.hpp>
#include <cstdint>

namespace igraphicslib {

class WindowPImpl : public sf::RenderWindow {
public:
    WindowPImpl(uint32_t width, uint32_t height, const char* title);
    ~WindowPImpl() override = default;
};

}  // namespace igraphicslib

#endif
