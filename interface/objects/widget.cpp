#include <interface/objects/widget.hpp>
#include "interface/application.hpp"

namespace interface::objects {

Widget::Widget(IWidget* parent, const geometry::Rect2<int32_t>& bounds) :
	IWidget(parent, bounds) {
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
	bounds_.x += delta.x;
	bounds_.y += delta.y;
}

void Widget::SetFocusIn()  {
	is_in_focus_ = true;
}

void Widget::SetFocusOut() {
	is_in_focus_ = false;
}

void SetFocusNextChild() {
	// ???
}

void Widget::SetFocusPrevChild() {
	// ???
}

void Widget::Resize(int32_t width, int32_t height)    {
	bounds_.w = width;
	bounds_.h = height;
}

void Widget::Resize(const geometry::Rect2<int32_t>& bounds) {
	bounds_ = bounds;
}

void Widget::AddChild(IWidget* child, manage::Priority priority) {
	children_manager_.Subscribe(child, priority);
}

void Widget::RemoveChild(IWidget* child) {
	children_manager_.Unsubscribe(child);
}

}  // namespace interface::objects
