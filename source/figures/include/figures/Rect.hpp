#ifndef MEPHI_REACTOR_SOURCE_FIGURES_INCLUDE_FIGURES_SOLID_RECT_HPP
#define MEPHI_REACTOR_SOURCE_FIGURES_INCLUDE_FIGURES_SOLID_RECT_HPP

#include <cstdlib>

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "common/ErrorHandle.hpp"
#include "vector/Vector.hpp"

namespace Mephi
{

class Rect {
    private:
        Mephi::Vector2d  leftCorner_;
        Mephi::Vector2d rightCorner_;
        sf::Color fillColor_;
        sf::Color outlineColor_;
        const double thickness_;
    public:
        Rect(const Mephi::Vector2d&  leftCorner, 
             const Mephi::Vector2d& rightCorner, 
             const sf::Color&         fillColor = sf::Color::Black, 
             const sf::Color&      outlineColor = sf::Color::Black,
             const double             thickness = 1)
            : leftCorner_{leftCorner}, rightCorner_{rightCorner}, fillColor_{fillColor}, 
              outlineColor_{outlineColor}, thickness_(thickness)
        {}

        [[nodiscard]] sf::RectangleShape GetSFRect() const;
        Common::Error Draw(sf::RenderWindow& window) const;
};

}
#endif /*MEPHI_REACTOR_SOURCE_FIGURES_INCLUDE_FIGURES_SOLID_RECT_HPP*/
