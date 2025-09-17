#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "molecule/MoleculeSquare.hpp"
#include "vector/Vector.hpp"

Common::Error Mephi::MoleculeSquare::Draw(sf::RenderWindow& window) const {
    sf::RectangleShape square(sf::Vector2f(side_, side_));
    square.setFillColor(color_);
    square.setPosition(static_cast<sf::Vector2f>(coord_));
    window.draw(square);
    
    return Common::Error::SUCCESS;
}
Common::Error Mephi::MoleculeSquare::Update(){
    coord_ += speed_;
    return Common::Error::SUCCESS;
}