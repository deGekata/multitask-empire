#include <interface/objects/iwidget.hpp>

namespace interface::objects {

IWidget::IWidget(IWidget* parent, const geometry::Rect2<int32_t>& bounds)
	: parent_(parent)
	, is_shown_(false)
	, is_in_focus_(false)
	, is_dragged_(false)
	, bounds_(bounds) {
}

}  // namespace interface::objects
