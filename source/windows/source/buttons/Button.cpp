#include "windows/buttons/Button.hpp"
#include "common/ErrorHandle.hpp"
#include "windows/Window.hpp"
#include <SFML/System/Vector2.hpp>

Common::Error Mephi::Button::Draw(sf::RenderWindow& window) const {
    ERROR_HANDLE(this->Mephi::Window::Draw(window));

    sf::Text printText = text_;
    printText.setPosition(
        static_cast<sf::Vector2f>(AbsoluteCoord()) 
      + printText.getPosition() 
      + sf::Vector2f(0, -5)
    );

    window.draw(printText);

    return Common::Error::SUCCESS;
}

bool Mephi::Button::OnMousePress(Mephi::EventMouseButton event) {
    if (isHovered_ && event.button == ACTIVATION_BUTTON_) {
        isPressed_ = true;
    }

    return Mephi::Window::OnMousePress(event);
}

bool Mephi::Button::OnMouseUnpress(Mephi::EventMouseButton event) {
    if (event.button == ACTIVATION_BUTTON_) {
        isPressed_ = false;
    }

    return Mephi::Window::OnMouseUnpress(event);
}

Common::Error Mephi::Button::Update() {
    if (isPressed_) {
        rect_.GetFillColor() = pressedColor_;
    } else {
        rect_.GetFillColor() = defaultColor_;
    }

    ERROR_HANDLE(Mephi::Window::Update());

    return Common::Error::SUCCESS;
}