#include "windows/Window.hpp"
#include "common/ErrorHandle.hpp"
#include "vector/Vector.hpp"

#include <SFML/Window/Mouse.hpp>

bool Mephi::Window::CheckPressed(const Mephi::Vector2i& mousePos, const sf::Mouse::Button& mouseButton) const {
    return rect_.GetLeftCorner().x < mousePos.x && mousePos.x < rect_.GetRightCorner().x 
        && rect_.GetLeftCorner().y < mousePos.y && mousePos.y < rect_.GetRightCorner().y
        && sf::Mouse::isButtonPressed(mouseButton);
}

bool Mephi::Window::CheckHold(const Mephi::Vector2i& mousePos, const sf::Mouse::Button& mouseButton) {
    const bool isPressed = CheckPressed(mousePos, mouseButton);
    // std::cout << "isHold1 " << isHold_ << std::endl;
    // std::cout << "check pressed " << isPressed << std::endl;
    const bool result = isPressed && isHold_;
    isHold_ = isPressed;
    // std::cout << "isHold2 " << isHold_ << std::endl;
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

    for (auto& child : children_) {
        child->Move(shift);
    }
    return Common::Error::SUCCESS;
}

Mephi::Vector2i Mephi::Window::HandleDrag(const Mephi::Vector2i& curMousePos) {
    const Mephi::Vector2i shift(HandleMouseShift(curMousePos));
    bool isHold = CheckHold(curMousePos);

    if (isHold) {
        Move(static_cast<Mephi::Vector2d>(shift));
    }

    return Mephi::Vector2i(shift * (int)isHold);
}

Common::Error Mephi::Window::Draw(sf::RenderWindow& window) {
    ERROR_HANDLE(rect_.Draw(window));

    for (auto& child : children_) {
        ERROR_HANDLE(child->Draw(window));
    }

    return Common::Error::SUCCESS;
}

Common::Error Mephi::Window::AddChild(std::unique_ptr<Window> child) {
    children_.push_back(std::move(child));

    return Common::Error::SUCCESS;
}

Common::Error Mephi::Window::HandlePressed(const Mephi::Vector2i& mousePos) {
    for (auto& child : children_) {
        ERROR_HANDLE(child->HandlePressed(mousePos));
    }

    return Common::Error::SUCCESS;
}