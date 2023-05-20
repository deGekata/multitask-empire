// Multitask-Empire
#include <interface/objects/iobject.hpp>
#include <interface/manage/children_manager.hpp>

#ifndef MULTITASK_EMPIRE_INTERFACE_OBJECTS_IWIDGET_HPP
#define MULTITASK_EMPIRE_INTERFACE_OBJECTS_IWIDGET_HPP

namespace interface::objects {

// Basic drawable
class IWidget : public IObject {
public:
	explicit IWidget(IWidget* parent = nullptr, const geometry::Rect2<int32_t>& bounds = {0, 0, 0, 0});

	~IWidget() override = default;

	// Time Event
	bool OnTimerTick(utility::Time time_data) override;

	// Every widget knows how to draw itself
	virtual void Draw() = 0;

	// Visibility
	virtual void Show() = 0;
	virtual void Hide() = 0;

	// Dragging
	virtual void DragEnter() = 0;
	virtual void DragLeave() = 0;
	virtual void DragMove(const geometry::Vector2<int32_t>& delta) = 0;

	// Focus
	virtual void SetFocusIn()  = 0;
	virtual void SetFocusOut() = 0;

	virtual void SetFocusNextChild() = 0;
	virtual void SetFocusPrevChild() = 0;

	// Bounds
	virtual void Resize(int32_t width, int32_t height)          = 0;
	virtual void Resize(const geometry::Rect2<int32_t>& bounds) = 0;

	// Children
	virtual void AddChild(IWidget* child, manage::Priority priority = 0) = 0;
	virtual void RemoveChild(IWidget* child) = 0;

protected:
    manage::ChildrenManager children_manager_;

    IWidget* parent_;

    bool is_shown_;
    bool is_in_focus_;
    bool is_dragged_;

    geometry::Rect2<int32_t> bounds_;
};

}  // namespace interface::objects

#endif  // MULTITASK_EMPIRE_INTERFACE_OBJECTS_IWIDGET_HPP
