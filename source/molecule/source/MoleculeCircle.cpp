#include <typeinfo>

#include <SFML/Graphics/CircleShape.hpp>

#include "molecule/MoleculeCircle.hpp"
#include "vector/Vector.hpp"

Common::Error Mephi::MoleculeCircle::Draw(sf::RenderWindow& window) const {
    sf::CircleShape circle(radius_);
    circle.setFillColor(color_);
    circle.setPosition(static_cast<sf::Vector2f>(coord_));
    window.draw(circle);
    
    return Common::Error::SUCCESS;
}
Common::Error Mephi::MoleculeCircle::Update(){
    coord_ += speed_;
    return Common::Error::SUCCESS;
}