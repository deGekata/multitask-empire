#include <interface/objects/button.hpp>

namespace interface::objects {

Button::Button(IWidget* parent, const geometry::Rect2<int32_t>& bounds, const igraphicslib::Text& text)
	: Widget(parent, bounds)
	, text_(text) {
}

}  // namespace interface::objects
