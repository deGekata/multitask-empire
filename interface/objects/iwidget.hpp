// Multitask-Empire
#include <interface/objects/iobject.hpp>
#include <interface/manage/children_manager.hpp>

// C++ standart library
#include <functional>

#ifndef MULTITASK_EMPIRE_INTERFACE_OBJECTS_IWIDGET_HPP
#define MULTITASK_EMPIRE_INTERFACE_OBJECTS_IWIDGET_HPP

namespace interface::objects {

// Basic drawable
class IWidget : public IObject {
public:
	explicit IWidget(IWidget* parent);
	explicit IWidget(const geometry::Rect2<uint32_t>& bounds);
	IWidget(IWidget* parent, const geometry::Rect2<uint32_t>& bounds);

	~IWidget() override;

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
	virtual void Resize(uint32_t width, uint32_t height)         = 0;
	virtual void Resize(const geometry::Rect2<uint32_t>& bounds) = 0;

	virtual geometry::Rect2<uint32_t> Bounds() const = 0;

	virtual bool Intersects(igraphicslib::Point point) = 0;
	virtual bool Intersects(geometry::Rect2<uint32_t> bounds) = 0;

	// Children
	virtual void AddChild(IWidget* child, manage::Priority priority = 0) = 0;
	virtual void RemoveChild(IWidget* child) = 0;

	// Parent
	virtual IWidget* Parent() const = 0;

	// Interaction
	virtual void SetOnHoverIn (std::function<void ()> on_hover_in)  = 0;
	virtual void SetOnHoverOut(std::function<void ()> on_hover_out) = 0;
	virtual void SetOnClick   (std::function<void ()> on_click)     = 0;

protected:
    manage::ChildrenManager children_manager_;

    IWidget* parent_;

    bool is_shown_;
    bool is_in_focus_;
    bool is_dragged_;
    bool is_hovered_;

    geometry::Rect2<uint32_t> bounds_;

    std::function<void ()> on_hover_in_;
    std::function<void ()> on_hover_out_;
    std::function<void ()> on_click_;
};

}  // namespace interface::objects

#endif  // MULTITASK_EMPIRE_INTERFACE_OBJECTS_IWIDGET_HPP
