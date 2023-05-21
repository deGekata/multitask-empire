#include <interface/objects/button.hpp>
#include <interface/application.hpp>

namespace interface::objects {

Button::Button(IWidget* parent)
	: Widget(parent) {
}

Button::Button(const geometry::Rect2<uint32_t>& bounds)
	: Widget(bounds) {
}

Button::Button(IWidget* parent, const geometry::Rect2<uint32_t>& bounds)
    : Widget(parent, bounds) {
}
 	
void Button::Draw() {
	if (image_ != nullptr) {
		image_->Draw();
	}

	if (label_ != nullptr) {
		label_->Draw();
	}
}

bool Button::OnButtonRelease(igraphicslib::MouseButtonEventData button_data) {
	if (!Intersects(button_data.point)) {
		return false;
	}

	callback_();
	return true;
}

void Button::SetImage(const std::string& image) {
	delete image_;
	image_ = new Image(this, image);
}

void Button::SetLabel(const std::string& label) {
	delete label_;
	label_ = new Label(this, label);
	label_->SetCharacterSize(bounds_.h / 2);
}

void Button::SetCallback(const std::function<void ()>& callback) {
	callback_ = callback;
}

}  // namespace interface::objects
