// Multitask-Empire
#include <interface/objects/widget.hpp>
#include <graphics/quick.hpp>

#ifndef MULTITASK_EMPIRE_INTERFACE_OBJECTS_IMAGE_HPP
#define MULTITASK_EMPIRE_INTERFACE_OBJECTS_IMAGE_HPP

namespace interface::objects {

class Image : public Widget {
public:
    explicit Image(IWidget* parent, const std::string& path);
    explicit Image(const geometry::Rect2<uint32_t>& bounds, const std::string& path);
    Image(IWidget* parent, const geometry::Rect2<uint32_t>& bounds, const std::string& path);

    void Draw() override;

    std::string Path() const;

    ~Image() override = default;

protected:
	std::string path_;
    igraphicslib::Texture texture_;
};

}  // namespace interface::objects

#endif  // MULTITASK_EMPIRE_INTERFACE_OBJECTS_IMAGE_HPP
