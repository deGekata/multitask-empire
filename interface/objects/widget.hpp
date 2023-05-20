// Multitask-Empire
#include <interface/objects/iwidget.hpp>
#include <interface/manage/children_manager.hpp>

#ifndef MULTITASK_EMPIRE_INTERFACE_OBJECTS_WIDGET_HPP
#define MULTITASK_EMPIRE_INTERFACE_OBJECTS_IWIDGET_HPP

namespace interface::objects {

class Widget : public IWidget {
public:
	explicit Widget(IWidget* parent, const geometry::Rect2u& bounds = {0, 0, 0, 0});

	~Widget() override = default;

protected:
	manage::ChildrenManager children_manager_;

	IWidget* parent_;

	bool is_shown_;
	bool is_in_focus_;
	bool is_dragged;

	geometry::Rect2u bounds_;
};

}  // namespace interface::objects

#endif  // MULTITASK_EMPIRE_INTERFACE_OBJECTS_IWIDGET_HPP
