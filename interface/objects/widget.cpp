#include <interface/objects/widget.hpp>

namespace interface::objects {

Widget::Widget(IWidget* parent)
	: IWidget(parent) {
}

Widget::Widget(const geometry::Rect2<uint32_t>& bounds)
	: IWidget(bounds) {
}

Widget::Widget(IWidget* parent, const geometry::Rect2<uint32_t>& bounds)
	: IWidget(parent, bounds) {
}

bool Widget::OnMouseMove(igraphicslib::MouseMoveEventData  mouse_data) {
	if (!is_shown_) {
		return false;
	}

	return children_manager_.ProcessMouseMovement(mouse_data);
}

bool Widget::OnWheelScroll(igraphicslib::MouseWheelEventData wheel_data) {
	if (!is_shown_) {
		return false;
	}

	return children_manager_.ProcessWheelScroll(wheel_data);
}

bool Widget::OnButtonPress(igraphicslib::MouseButtonEventData button_data) {
	if (!is_shown_) {
		return false;
	}

	return children_manager_.ProcessButtonPress(button_data);
}

bool Widget::OnButtonRelease(igraphicslib::MouseButtonEventData button_data) {
	if (!is_shown_) {
		return false;
	}

	return children_manager_.ProcessButtonRelease(button_data);
}

bool Widget::OnKeyPress(igraphicslib::KeyEventData key_data) {
	if (!is_shown_) {
		return false;
	}

	return children_manager_.ProcessKeyPress(key_data);
}

bool Widget::OnKeyRelease(igraphicslib::KeyEventData key_data) {
	if (!is_shown_) {
		return false;
	}

	return children_manager_.ProcessKeyRelease(key_data);
}

bool Widget::OnTimerTick(utility::Time time_data) {
	if (!is_shown_) {
		return false;
	}

	Draw();
	return true | children_manager_.ProcessTimerTick(time_data);
}

void Widget::Draw() {
	// ???
}

void Widget::Show() {
	is_shown_ = true;
}

void Widget::Hide() {
	is_shown_ = false;
}

void Widget::DragEnter() {
	is_dragged_ = true;
}

void Widget::DragLeave() {
	is_dragged_ = false;
}

void Widget::DragMove(const geometry::Vector2<int32_t>& delta) {
	int32_t new_x = static_cast<int32_t>(bounds_.x) + delta.x;
	int32_t new_y = static_cast<int32_t>(bounds_.y) + delta.y;

	bounds_.x = static_cast<uint32_t>(new_x);
	bounds_.y = static_cast<uint32_t>(new_y);
}

void Widget::SetFocusIn()  {
	is_in_focus_ = true;
}

void Widget::SetFocusOut() {
	is_in_focus_ = false;
}

void Widget::SetFocusNextChild() {
	// ???
}

void Widget::SetFocusPrevChild() {
	// ???
}

void Widget::Resize(uint32_t width, uint32_t height)    {
	bounds_.w = width;
	bounds_.h = height;
}

void Widget::Resize(const geometry::Rect2<uint32_t>& bounds) {
	bounds_ = bounds;
}

geometry::Rect2<uint32_t> Widget::Bounds() const {
	return bounds_;
}

bool Widget::Intersects(igraphicslib::Point point) {
	geometry::Rect2<uint32_t> abs_bounds = AbsoluteBounds();

	if (static_cast<uint32_t>(point.x) <= abs_bounds.x ||
		static_cast<uint32_t>(point.x) >= abs_bounds.x + abs_bounds.w) {
		return false;
	}

	if (static_cast<uint32_t>(point.y) <= abs_bounds.y ||
		static_cast<uint32_t>(point.y) >= abs_bounds.y + abs_bounds.h) {
		return false;
	}

	return true;
}

bool Widget::Intersects(geometry::Rect2<uint32_t> bounds) {
	return Intersects(bounds.GetCornerGG()) || Intersects(bounds.GetCornerGL()) ||
	       Intersects(bounds.GetCornerLL()) || Intersects(bounds.GetCornerLG());
}

void Widget::AddChild(IWidget* child, manage::Priority priority) {
	children_manager_.Subscribe(child, priority);
}

void Widget::RemoveChild(IWidget* child) {
	children_manager_.Unsubscribe(child);
}

geometry::Rect2<uint32_t> Widget::AbsoluteBounds() const {
	geometry::Rect2<uint32_t> abs_bounds = bounds_;

	IWidget* parent = parent_;
	while (parent != nullptr) {
		abs_bounds.x += parent->Bounds().x;
		abs_bounds.y += parent->Bounds().y;
		parent = parent->Parent();
	}

	return abs_bounds;
}

IWidget* Widget::Parent() const {
	return parent_;
}

}  // namespace interface::objects
