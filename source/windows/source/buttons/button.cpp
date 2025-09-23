#include "windows/buttons/button.hpp"

Common::Error Mephi::Button::Draw(sf::RenderWindow& window) {

    if (CheckPressed(Mephi::Vector2i(sf::Mouse::getPosition(window)))) {
        rect_.GetFillColor() = pressedColor_;
        std::cerr << "pressed" << std::endl;
    } else {
        rect_.GetFillColor() = defaultColor_;
        std::cerr << "default" << std::endl;
    }

    rect_.Draw(window);

    return Common::Error::SUCCESS;
}