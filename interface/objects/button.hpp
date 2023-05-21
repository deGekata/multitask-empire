// Multitask-Empire
#include <interface/objects/image.hpp>
#include <interface/objects/label.hpp>

#ifndef MULTITASK_EMPIRE_INTERFACE_OBJECTS_BUTTON_HPP
#define MULTITASK_EMPIRE_INTERFACE_OBJECTS_BUTTON_HPP

namespace interface::objects {

class Button : public Widget {
public:
	explicit Button(IWidget* parent);
    explicit Button(const geometry::Rect2<uint32_t>& bounds);
    Button(IWidget* parent, const geometry::Rect2<uint32_t>& bounds);

    void Draw() override;

    void Resize(uint32_t width, uint32_t height)         override;
    void Resize(const geometry::Rect2<uint32_t>& bounds) override;

    void SetImage(const std::string& image);
    void SetLabel(const std::string& label);

    // Label
    void SetColor(const igraphicslib::Color& color);
    void SetFont(const std::string& path);
    void SetCharacterSize(uint32_t size);

    ~Button() override = default;

protected:
	Image* image_{nullptr};
	Label* label_{nullptr};
};

}  // namespace interface::objects

#endif  // MULTITASK_EMPIRE_INTERFACE_OBJECTS_BUTTON_HPP
