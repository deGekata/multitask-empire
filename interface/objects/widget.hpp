// Multitask-Empire
#include <interface/objects/iwidget.hpp>

#ifndef MULTITASK_EMPIRE_INTERFACE_OBJECTS_WIDGET_HPP
#define MULTITASK_EMPIRE_INTERFACE_OBJECTS_WIDGET_HPP

namespace interface::objects {

class Widget : public IWidget {
public:
    explicit Widget(IWidget* parent = nullptr, const geometry::Rect2<int32_t>& bounds = {0, 0, 0, 0});

    ~Widget() override = default;

    // Mouse Events
    bool OnMouseMove  (igraphicslib::MouseMoveEventData  mouse_data) override;
    bool OnWheelScroll(igraphicslib::MouseWheelEventData wheel_data) override;

    bool OnButtonPress  (igraphicslib::MouseButtonEventData button_data) override;
    bool OnButtonRelease(igraphicslib::MouseButtonEventData button_data) override;

    // Keyboard Events
    bool OnKeyPress  (igraphicslib::KeyEventData key_data) override;
    bool OnKeyRelease(igraphicslib::KeyEventData key_data) override;

    // Time Event
    bool OnTimerTick(utility::Time time_data) override;

    // Every widget knows how to draw itself
    void Draw() override;

    // Visibility
    void Show() override;
    void Hide() override;

    // Dragging
    void DragEnter() override;
    void DragLeave() override;
    void DragMove(const geometry::Vector2<int32_t>& delta) override;

    // Focus
    void SetFocusIn()  override;
    void SetFocusOut() override;

    void SetFocusNextChild() override;
    void SetFocusPrevChild() override;

    // Bounds
    void Resize(int32_t width, int32_t height)          override;
    void Resize(const geometry::Rect2<int32_t>& bounds) override;

    // Children
    void AddChild(IWidget* child, manage::Priority priority) override;
    void RemoveChild(IWidget* child) override;
};

}  // namespace interface::objects

#endif  // MULTITASK_EMPIRE_INTERFACE_OBJECTS_WIDGET_HPP
