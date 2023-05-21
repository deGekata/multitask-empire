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

void Button::Resize(uint32_t width, uint32_t height) {
	bounds_.w = width;
	bounds_.h = height;

	if (label_ != nullptr) {
		label_->Resize(width, height);
	}

	if (image_ != nullptr) {
		image_->Resize(width, height);
	}
}

void Button::Resize(const geometry::Rect2<uint32_t>& bounds) {
	bounds_ = bounds;

	if (label_ != nullptr) {
		label_->Resize(bounds);
	}

	if (image_ != nullptr) {
		image_->Resize(bounds);
	}
}

void Button::SetImage(const std::string& image) {
	delete image_;
	image_ = new Image(this, image);
}

void Button::SetLabel(const std::string& label) {
	delete label_;
	label_ = new Label(this, label);
	label_->SetCharacterSize(bounds_.h);
}

void Button::SetColor(const igraphicslib::Color& color) {
	if (label_ == nullptr) {
		return;
	}

	label_->SetColor(color);
}

void Button::SetFont(const std::string& path) {
	if (label_ == nullptr) {
		return;
	}

	label_->SetFont(path);
}

void Button::SetCharacterSize(uint32_t size) {
	if (label_ == nullptr) {
		return;
	}

	label_->SetCharacterSize(size);
}

}  // namespace interface::objects
