#include "windows/Window.hpp"
#include "common/ErrorHandle.hpp"
#include "vector/Vector.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Mouse.hpp>

Mephi::Vector2d Mephi::Window::AbsoluteCoord() const noexcept {
    if (parent_ == nullptr)
        return rect_.GetLeftCorner();

    return parent_->AbsoluteCoord() + rect_.GetLeftCorner();
}

Common::Error Mephi::Window::Draw(sf::RenderWindow& window) const {
    for (const auto& child : children_) {
        ERROR_HANDLE(child->Draw(window));
    }

    sf::RectangleShape sfRect(rect_.GetSFRect());
    sfRect.setPosition(static_cast<sf::Vector2f>(AbsoluteCoord()));

    window.draw(sfRect);

    return Common::Error::SUCCESS;
}