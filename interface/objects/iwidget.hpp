// Multitask-Empire
#include <interface/objects/iobject.hpp>

#ifndef MULTITASK_EMPIRE_INTERFACE_OBJECTS_IWIDGET_HPP
#define MULTITASK_EMPIRE_INTERFACE_OBJECTS_IWIDGET_HPP

namespace interface::objects {

class IWidget : public IObject {
public:
	 ~IWidget() override = default;

	// Every widget knows how to draw itself
	virtual void Draw() = 0;

	// Visibility
	virtual void Show() = 0;
	virtual void Hide() = 0;

	// Dragging
	virtual void DragEnter() = 0;
	virtual void DragLeave() = 0;
	virtual void DragMove(const geometry::Vector2i& delta) = 0;

	// Focus
	virtual void SetFocusIn() = 0;
	virtual void SetFocusOut() = 0;

	virtual void SetFocusNextChild() = 0;
	virtual void SetFocusPrevChild() = 0;

	// Bounds
	virtual void Resize(size_t width, size_t height) = 0;
};

}  // namespace interface::objects

#endif  // MULTITASK_EMPIRE_INTERFACE_OBJECTS_IWIDGET_HPP
