// Multitask-Empire
#include <interface/objects/widget.hpp>
#include <graphics/quick.hpp>

#ifndef MULTITASK_EMPIRE_INTERFACE_OBJECTS_LABEL_HPP
#define MULTITASK_EMPIRE_INTERFACE_OBJECTS_LABEL_HPP

namespace interface::objects {

class Label : public Widget {
public:
    explicit Label(IWidget* parent, const std::string& text);
    explicit Label(const geometry::Rect2<uint32_t>& bounds, const std::string& text);
    Label(IWidget* parent, const geometry::Rect2<uint32_t>& bounds, const std::string& text);

    void Draw() override;

    void SetColor(const igraphicslib::Color& color);
    void SetFont(const std::string& path);
    void SetCharacterSize(uint32_t size);

    std::string Text() const;

    ~Label() override = default;

protected:
	std::string text_;
    igraphicslib::Text renderable_text_;
};

}  // namespace interface::objects

#endif  // MULTITASK_EMPIRE_INTERFACE_OBJECTS_LABEL_HPP
