#include <interface/objects/image.hpp>

#include <interface/application.hpp>

namespace interface::objects {

Image::Image(IWidget* parent, const std::string& path)
	: Widget(parent)
	, path_(path)
	, texture_(path_.c_str()) {
}

Image::Image(const geometry::Rect2<uint32_t>& bounds, const std::string& path)
	: Widget(bounds)
	, path_(path)
	, texture_(path_.c_str()) {
}

Image::Image(IWidget* parent, const geometry::Rect2<uint32_t>& bounds, const std::string& path)
    : Widget(parent, bounds)
    , path_(path)
    , texture_(path_.c_str()) {
}

void Image::Draw() {
    geometry::Rect2<uint32_t> abs_bounds = AbsoluteBounds();
    igraphicslib::Sprite sprite(texture_, abs_bounds);
    igraphicslib::Point point = {static_cast<int32_t>(abs_bounds.x), static_cast<int32_t>(abs_bounds.y)};
    Application::Window()->DrawSprite(point, sprite);
}

std::string Image::Path() const {
	return path_;
}

}  // namespace interface::objects
