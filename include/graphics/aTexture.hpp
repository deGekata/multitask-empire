#ifndef IGRAPHICS_ATEXTURE_HPP
#define IGRAPHICS_ATEXTURE_HPP

namespace igraphicslib {
    
class RenderSurface;
class Image;
class Texture {
    class Texture_;
    Texture_* tp_;
    friend class Sprite;
    friend class RenderSurface;
    explicit Texture(const Texture_& tp);
    explicit Texture(Texture_* tp) : tp_(tp) {
    }

public:
    Texture();
    //NOLINTNEXTLINE
    Texture(const char* filename);

    Texture(const Texture& texture);
    Texture& operator=(const Texture& texture);

    ~Texture();

    bool LoadFromFile(const char* filename);
    void LoadFromImage(const Image& img);
    void SetRepeated(bool) const;
};
}  // namespace igraphicslib

#endif /* IGRAPHICS_ATEXTURE_HPP */
