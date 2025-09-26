#include "windows/buttons/Button.hpp"
#include "common/ErrorHandle.hpp"

Common::Error Mephi::Button::HandlePressed(const Mephi::Vector2i& mousePos) {
    if (CheckPressed(Mephi::Vector2i(mousePos))) {
        rect_.GetFillColor() = pressedColor_;
    } else {
        rect_.GetFillColor() = defaultColor_;
    }

    return Common::Error::SUCCESS;
}

Common::Error Mephi::Button::Draw(sf::RenderWindow& window) {
    
    ERROR_HANDLE(Mephi::Window::Draw(window));
    window.draw(text_);

    return Common::Error::SUCCESS;
}