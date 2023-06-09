#include "graphics/SFML/SFMLWindow.hpp"
#include "graphics/aWindow.hpp"
#include "graphics/SFML/SFMLText.hpp"
#include "graphics/SFML/SFMLRenderSurface.hpp"
#include "graphics/SFML/SFMLSprite.hpp"

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <cassert>
#include <iostream>

namespace igraphicslib {

WindowPImpl::WindowPImpl(uint32_t width, uint32_t height, const char* title)
    : sf::RenderWindow(sf::VideoMode(width, height), title) {
}

// WindowPImpl::~WindowPImpl() {
// }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

Window::Window(uint32_t width, uint32_t height, const char* title) : wp_(new WindowPImpl(width, height, title)) {
    assert(wp_ != nullptr);
    
}

Window::~Window() {
    delete wp_;
    wp_ = nullptr;
}

void Window::Resize(uint32_t width, uint32_t height) {
    wp_->setSize(sf::Vector2u(width, height));
}

void Window::Show() {
    wp_->setVisible(true);
}

void Window::Hide() {
    wp_->setVisible(false);
}

void Window::Clear(Color color) const {
    wp_->clear(sf::Color(color.R(), color.G(), color.B(), color.A()));
}

void Window::DrawText(const Text& text) const {
    wp_->draw(*text.tp_);
}

void Window::Update() const {
    wp_->display();
}

void Window::DrawLine(Point p1, Point p2, Color color) const {
    sf::Vertex lines[2] = {};
    lines[0].position = sf::Vector2f(static_cast<float>(p1.x), static_cast<float>(p1.y));
    lines[1].position = sf::Vector2f(static_cast<float>(p2.x), static_cast<float>(p2.y));

    lines[0].color = lines[1].color = sf::Color(color.R(), color.G(), color.B(), color.A());

    wp_->draw(lines, 2, sf::Lines);
}

void Window::DrawRect(const Rect& rect, Color color) const {
    sf::RectangleShape sf_rect(sf::Vector2f(static_cast<float>(rect.w), static_cast<float>(rect.h)));
    sf_rect.setPosition(static_cast<float>(rect.x), static_cast<float>(rect.y));
    sf_rect.setFillColor(sf::Color(color.R(), color.G(), color.B(), color.A()));
    wp_->draw(sf_rect);
}

void Window::DrawPoint(Point p, Color color) const {
    sf::Vertex vertex({static_cast<float>(p.x), static_cast<float>(p.y)}, sf::Color(color.R(), color.G(), color.B(), color.A()));
    wp_->draw(&vertex, 1, sf::Points);
}

void Window::DrawSurf(Point pt, const RenderSurface& surf) const {
    sf::Sprite sprite(surf.rsp_->getTexture());
    sprite.setOrigin(0, static_cast<float>(sprite.getTextureRect().height));
    sprite.setScale(1, -1);
    sprite.setPosition({static_cast<float>(pt.x), static_cast<float>(pt.y)});
    wp_->draw(sprite);
}

void Window::DrawSurf(Point pt, const RenderSurface& surf, const Rect& rect) const {
    sf::Sprite sprite(surf.rsp_->getTexture());
    sprite.setTextureRect({static_cast<int>(rect.x), static_cast<int>(rect.h / 2 - rect.y), static_cast<int>(rect.w),
                           static_cast<int>(rect.h)});
    sprite.setOrigin(0, static_cast<float>(rect.h));
    sprite.setScale(1, -1);
    sprite.setPosition({static_cast<float>(pt.x), static_cast<float>(pt.y)});
    wp_->draw(sprite);
}

static MouseButton GetMouseButton(sf::Mouse::Button button) {
    switch (button) {

        case sf::Mouse::Left:
            return MouseButton::Left;
        case sf::Mouse::Right:
            return MouseButton::Right;
        case sf::Mouse::Middle:
            return MouseButton::Middle;
        case sf::Mouse::XButton1:
        case sf::Mouse::XButton2:
        case sf::Mouse::ButtonCount:
            return MouseButton::Other;
    }
    assert(0 && "Wrong switch");
}

bool Window::PollEvent(Event& event) {
    sf::Event sf_event;
    if (wp_->pollEvent(sf_event)) {
        switch (sf_event.type) {

            case sf::Event::KeyPressed:
                event.type = EventType::KeyPressed;
                event.ked = *reinterpret_cast<KeyEventData*>(
                    &sf_event.key);  // I'm personally created very simmilar to SFML prototypes, so I've decided to make
                                    // reinterperter cast.
                break;

            case sf::Event::KeyReleased:
                event.type = EventType::KeyReleased;
                event.ked = *reinterpret_cast<KeyEventData*>(&sf_event.key);
                break;

            case sf::Event::Closed:
                // event.type = EventType::Quited;
                break;

            case sf::Event::MouseWheelMoved:
                event.type = EventType::MouseWheeled;
                event.mwed.delta = sf_event.mouseWheel.delta;
                event.mwed.point = {sf_event.mouseWheel.x, sf_event.mouseWheel.y};
                break;

            case sf::Event::MouseButtonPressed:
                event.type = EventType::MouseButtonPressed;
                event.mbed.button = GetMouseButton(sf_event.mouseButton.button);
                event.mbed.point = {(sf_event.mouseButton.x), (sf_event.mouseButton.y)};
                break;
            case sf::Event::MouseButtonReleased:
                event.type = EventType::MouseButtonReleased;
                event.mbed.button = GetMouseButton(sf_event.mouseButton.button);
                event.mbed.point = {(sf_event.mouseButton.x), (sf_event.mouseButton.y)};
                break;
            case sf::Event::MouseMoved:
                event.type = EventType::MouseMoved;
                event.mmed.point = {(sf_event.mouseMove.x), (sf_event.mouseMove.y)};
                break;
            case sf::Event::Resized: {
                sf::View view = wp_->getDefaultView();
                view.setSize({static_cast<float>(sf_event.size.width), static_cast<float>(sf_event.size.height)});
                view.reset(sf::FloatRect(0, 0, static_cast<float>(sf_event.size.width),
                                         static_cast<float>(sf_event.size.height)));
                wp_->setView(view);
            }
                return false;
            case sf::Event::MouseWheelScrolled:
            case sf::Event::LostFocus:
            case sf::Event::GainedFocus:
            case sf::Event::TextEntered:
                event.type = EventType::TextEntered;
                event.text = sf_event.text.unicode;
                break;
            case sf::Event::MouseEntered:
            case sf::Event::MouseLeft:
            case sf::Event::JoystickButtonPressed:
            case sf::Event::JoystickButtonReleased:
            case sf::Event::JoystickMoved:
            case sf::Event::JoystickConnected:
            case sf::Event::JoystickDisconnected:
            case sf::Event::TouchBegan:
            case sf::Event::TouchMoved:
            case sf::Event::TouchEnded:
            case sf::Event::SensorChanged:
            case sf::Event::Count:
                // event.type = EventType::Other;
                return false;
        }
        return true;
    }
    return false;
}

void Window::DrawSprite(Point pt, const Sprite& sprite) const {
    sprite.sp_->move({static_cast<float>(pt.x), static_cast<float>(pt.y)});
    wp_->draw(*sprite.sp_);
    sprite.sp_->move({-static_cast<float>(pt.x), -static_cast<float>(pt.y)});
}

void Window::SetActive(bool active) const {
    wp_->setActive(active);
}

igraphicslib::Rect Window::GetSurfRect() const {
    sf::Vector2u v = wp_->getSize();
    return {0, 0, v.x, v.y};
}

void Window::DrawHolRect(Point p1, Point p2, Color color) const {
    sf::Vertex quad[5];
    quad[0].position = sf::Vector2f(static_cast<float>(p1.x) + 0.5f, static_cast<float>(p1.y));
    quad[1].position = sf::Vector2f(static_cast<float>(p1.x) + 0.5f, static_cast<float>(p2.y));
    quad[2].position = sf::Vector2f(static_cast<float>(p2.x) + 0.5f, static_cast<float>(p2.y));
    quad[3].position = sf::Vector2f(static_cast<float>(p2.x) + 0.5f, static_cast<float>(p1.y));
    quad[4].position = sf::Vector2f(static_cast<float>(p1.x) + 0.5f, static_cast<float>(p1.y));

    quad[0].color = quad[1].color = quad[2].color = quad[3].color = quad[4].color = sf::Color(color.R(), color.G(), color.B(), color.A());

    wp_->draw(quad, 5, sf::LinesStrip);
}

static sf::PrimitiveType SfPrimitive(PrimitiveType type) {
    switch (type) {

        case PrimitiveType::Points:
            return sf::Points;
        case PrimitiveType::Lines:
            return sf::Lines;
        case PrimitiveType::StripLines:
            return sf::LineStrip;
        case PrimitiveType::Triangles:
            return sf::Triangles;
        case PrimitiveType::StripTriangles:
            return sf::TriangleStrip;
        case PrimitiveType::Quads:
            return sf::Quads;
    }
    assert(0 && "Bad primitive type");
}

void Window::DrawVert(const Vertex* vert, size_t size, PrimitiveType type) const {
    auto sf_vert = new sf::Vertex[size];  // FIX:
    for (size_t i = 0; i < size; ++i) {
        sf_vert[i].position =
            sf::Vector2f(static_cast<float>(vert[i].point.x) + 0.5f, static_cast<float>(vert[i].point.y));
        sf_vert[i].color = sf::Color(vert[i].color.R(), vert[i].color.G(), vert[i].color.B(), vert[i].color.A());
    }
    wp_->draw(sf_vert, size, SfPrimitive(type));
    delete[] sf_vert;
}

void Window::DrawTriang(Vertex vert[3]) const {
    sf::Vertex triag[3];
    for (size_t i = 0; i < 3; ++i) {
        triag[i].position =
            sf::Vector2f(static_cast<float>(vert[i].point.x) + 0.5f, static_cast<float>(vert[i].point.y));
        triag[i].color = sf::Color(vert[i].color.R(), vert[i].color.G(), vert[i].color.B(), vert[i].color.A());
    }
    wp_->draw(triag, 3, sf::Triangles);
}

// void Window::setTopmost(bool topmost)
// {
//     wp_->setTopmost(topmost);
// }

}  // namespace igraphicslib
