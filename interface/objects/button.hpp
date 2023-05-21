// // Multitask-Empire
// #include <interface/objects/image.hpp>
// #include <interface/objects/label.hpp>

// // C++ standart library
// #include <functional>

// #ifndef MULTITASK_EMPIRE_INTERFACE_OBJECTS_BUTTON_HPP
// #define MULTITASK_EMPIRE_INTERFACE_OBJECTS_BUTTON_HPP

// namespace interface::objects {

// class Button : public Widget {
// public:
// 	explicit Button(IWidget* parent);
//     explicit Button(const geometry::Rect2<uint32_t>& bounds);
//     Button(IWidget* parent, const geometry::Rect2<uint32_t>& bounds);

//     void Draw() override;

//     void Setimage(const std::string& image);
//     void SetButton(const std::string& label);
//     void SetCallback(const std::function<void ()>& callback);

//     ~Button() override = default;

// protected:
// 	Image* image_{nullptr};
// 	Label* label_{nullptr};

// 	std::function<void ()> callback_;
// };

// }  // namespace interface::objects

// #endif  // MULTITASK_EMPIRE_INTERFACE_OBJECTS_BUTTON_HPP
