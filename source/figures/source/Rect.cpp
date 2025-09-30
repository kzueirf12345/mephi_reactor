#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include "figures/Rect.hpp"
#include "common/ErrorHandle.hpp"

sf::RectangleShape Mephi::Rect::GetSFRect() const {
    sf::RectangleShape rect(static_cast<sf::Vector2f>(size_));

    rect.setPosition(static_cast<sf::Vector2f>(leftCorner_));
    rect.setFillColor(fillColor_);
    rect.setOutlineColor(outlineColor_);
    rect.setOutlineThickness(thickness_);

    return rect;
}

Common::Error Mephi::Rect::Draw(sf::RenderWindow& window) const {
    window.draw(GetSFRect());
    return Common::Error::SUCCESS;
}