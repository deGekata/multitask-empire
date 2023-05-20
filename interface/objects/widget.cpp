#include <interface/objects/widget.hpp>

namespace interface::objects {

Widget::Widget(IWidget* parent, const geometry::Rect2u& bounds)
	: parent_(parent)
	, is_shown_(false)
	, is_in_focus_(false)
	, is_dragged(false)
	, bounds_(bounds) {
}

}  // namespace interface::objects
