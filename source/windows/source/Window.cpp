#include "windows/Window.hpp"
#include "vector/Vector.hpp"

#include <SFML/Window/Mouse.hpp>

bool Mephi::Window::CheckPressed(const Mephi::Vector2i& mousePos) const {
    return rect_.GetLeftCorner().x < mousePos.x && mousePos.x < rect_.GetRightCorner().x 
        && rect_.GetLeftCorner().y < mousePos.y && mousePos.y < rect_.GetRightCorner().y
        && sf::Mouse::isButtonPressed(MOVE_BUTTON_);
}

bool Mephi::Window::CheckHold(const Mephi::Vector2i& mousePos) {
    const bool isPressed = CheckPressed(mousePos);
    const bool result = isPressed && isHold_;
    isHold_ = isPressed;
    return result;
}

Mephi::Vector2i Mephi::Window::HandleMouseShift(const Mephi::Vector2i& curMousePos) {
    Mephi::Vector2i result(prevMousePos_ == Mephi::Window::POISON_MOUSE_POS_ 
                         ? Mephi::Window::POISON_MOUSE_POS_ 
                         : Mephi::Vector2i(curMousePos - prevMousePos_)
    );
    prevMousePos_ = curMousePos;
    return result;
}

Common::Error Mephi::Window::Move(const Mephi::Vector2d& shift) {
    rect_.GetLeftCorner()  += shift;
    rect_.GetRightCorner() += shift;
    return Common::Error::SUCCESS;
}

Mephi::Vector2i Mephi::Window::HandleDrag(const Mephi::Vector2i& curMousePos) {
    for (auto& child : children_) {
        child->HandleDrag(curMousePos);
    }

    const Mephi::Vector2i shift(HandleMouseShift(curMousePos));
    bool isHold = CheckHold(curMousePos);

    if (isHold) {
        Move(static_cast<Mephi::Vector2d>(shift));
    }

    return Mephi::Vector2i(shift * (int)isHold);
}