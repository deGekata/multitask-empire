#include <interface/objects/label.hpp>
#include <interface/application.hpp>

namespace interface::objects {

Label::Label(IWidget* parent, const std::string& text)
	: Widget(parent)
	, text_(text)
	, renderable_text_(text_.c_str()) {
}

Label::Label(const geometry::Rect2<uint32_t>& bounds, const std::string& text)
	: Widget(bounds)
	, text_(text)
	, renderable_text_(text_.c_str()) {
}

Label::Label(IWidget* parent, const geometry::Rect2<uint32_t>& bounds, const std::string& text)
    : Widget(parent, bounds)
    , text_(text)
    , renderable_text_(text_.c_str()) {
}

void Label::Draw() {
    geometry::Rect2<uint32_t> abs_bounds = AbsoluteBounds();
    renderable_text_.SetPosition(abs_bounds.x, abs_bounds.y);
    Application::Window()->DrawText(renderable_text_);
}

void Label::SetColor(const igraphicslib::Color& color) {
	renderable_text_.SetColor(color);
}

void Label::SetFont(const std::string& path) {
	renderable_text_.SetFont(path.c_str());
}

void Label::SetCharacterSize(uint32_t size) {
	renderable_text_.SetCharacterSize(size);
}

std::string Label::Text() const {
	return text_;
}

}  // namespace interface::objects
