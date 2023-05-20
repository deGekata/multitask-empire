// Multitask-Empire
#include <interface/objects/widget.hpp>
#include <graphics/aText.hpp>

#ifndef MULTITASK_EMPIRE_INTERFACE_OBJECTS_BUTTON_HPP
#define MULTITASK_EMPIRE_INTERFACE_OBJECTS_BUTTON_HPP

namespace interface::objects {

class Button : public Widget {
public:
    Button(IWidget* parent, const geometry::Rect2<int32_t>& bounds, const igraphicslib::Text& text);

    ~Button() override = default;

protected:
	igraphicslib::Text text_;
};

}  // namespace interface::objects

#endif  // MULTITASK_EMPIRE_INTERFACE_OBJECTS_BUTTON_HPP
