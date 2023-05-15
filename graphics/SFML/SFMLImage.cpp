#include "graphics/SFML/SFMLImage.hpp"

namespace igraphicslib {

Image::Image() : ip_(new Image_) {
}

Image::Image(const char* filename) : ip_(new Image_) {
    ip_->loadFromFile(filename);
}

Image::Image(const Image& oth) : ip_(new Image_(*oth.ip_)) {
}

Image& Image::operator=(const Image& oth) {
    *ip_ = *oth.ip_;
    return *this;
}

Image::~Image() {
    delete ip_;
    ip_ = nullptr;
}

void Image::LoadFromFile(const char* filename) {
    ip_->loadFromFile(filename);
}

const Color* Image::GetPixels() const {
    return reinterpret_cast<const Color*>(ip_->getPixelsPtr());
}

Color Image::GetPixel(unsigned int x, unsigned int y) const {
    sf::Color cl = ip_->getPixel(x, y);
    return {cl.r, cl.g, cl.b, cl.a};
}

void Image::Create(uint32_t w, uint32_t h, const Color& col) {
    ip_->create(w, h, sf::Color(col.R(), col.G(), col.B(), col.A()));
}

void Image::SetPixel(unsigned int x, unsigned int y, const Color& color) {
    ip_->setPixel(x, y, sf::Color(color.color()));
}

void Image::SaveToFile(const char* filename) {
    ip_->saveToFile(filename);
}

uint32_t Image::GetW() const {
    return ip_->getSize().x;
}
uint32_t Image::GetH() const {
    return ip_->getSize().y;
}

bool Image::IsCreated() const {
    return ip_->getSize() != sf::Vector2u{0, 0};
}

}  // namespace igraphicslib
