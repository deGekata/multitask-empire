#include <interface/objects/iwidget.hpp>
#include <interface/manage/event_manager.hpp>

namespace interface::objects {

IWidget::IWidget(IWidget* parent)
	: parent_(parent)
	, is_shown_(true)
	, is_in_focus_(false)
	, is_dragged_(false)
	, bounds_({0, 0, parent_->Bounds().w, parent->Bounds().h}) {

	parent_->AddChild(this);
}

IWidget::IWidget(const geometry::Rect2<uint32_t>& bounds)
	: parent_(nullptr)
	, is_shown_(true)
	, is_in_focus_(false)
	, is_dragged_(false)
	, bounds_(bounds) {

	manage::EventManager::Current()->Subscribe(this);
}

IWidget::IWidget(IWidget* parent, const geometry::Rect2<uint32_t>& bounds)
	: parent_(parent)
	, is_shown_(true)
	, is_in_focus_(false)
	, is_dragged_(false)
	, bounds_(bounds) {

	parent_->AddChild(this);
}

IWidget::~IWidget() {
	delete this;
}

}  // namespace interface::objects
