#ifndef IGRAPHICS_AIMAGE_HPP
#define IGRAPHICS_AIMAGE_HPP

#include "graphics/Color.hpp"

namespace igraphicslib {
class Image {
    class Image_;
    Image_* ip_ = nullptr;
    friend class Texture;

public:
    Image();
    Image(uint32_t w, uint32_t h, const Color& col = igraphicslib::colors::kWhite) : Image() {
        Create(w, h, col);
    }
    explicit Image(const char* filename);
    Image(const Image& oth);
    Image& operator=(const Image& oth);
    ~Image();

    void LoadFromFile(const char* filename);
    void SaveToFile(const char* filename);
    void Create(uint32_t w, uint32_t h, const Color& col = igraphicslib::colors::kWhite);

    const Color* GetPixels() const;
    Color GetPixel(unsigned int x, unsigned int y) const;
    void SetPixel(unsigned int x, unsigned int y, const Color& color);

    void SetScale(double scale);

    uint32_t GetW() const;
    uint32_t GetH() const;

    bool IsCreated() const;
};
}  // namespace igraphicslib

#endif /* IGRAPHICS_AIMAGE_HPP */
