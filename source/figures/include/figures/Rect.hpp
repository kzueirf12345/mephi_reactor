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
    protected:
        Mephi::Vector2d  leftCorner_;
        Mephi::Vector2d rightCorner_;
        sf::Color fillColor_;
        sf::Color outlineColor_;
        const double thickness_;
    public:
        Rect(const Mephi::Vector2d&  leftCorner, 
             const Mephi::Vector2d& rightCorner, 
             const sf::Color&         fillColor = sf::Color(240, 240, 240), 
             const sf::Color&      outlineColor = sf::Color::Black,
             const double             thickness = 1)
            : leftCorner_{leftCorner}, rightCorner_{rightCorner}, fillColor_{fillColor}, 
              outlineColor_{outlineColor}, thickness_(thickness)
        {}

        [[nodiscard]] sf::RectangleShape GetSFRect() const;
        Common::Error Draw(sf::RenderWindow& window) const;
        
        [[nodiscard]] const Mephi::Vector2d& GetLeftCorner()   const noexcept {return leftCorner_;}
        [[nodiscard]]       Mephi::Vector2d& GetLeftCorner()         noexcept {return leftCorner_;}
        [[nodiscard]] const Mephi::Vector2d& GetRightCorner()  const noexcept {return rightCorner_;}
        [[nodiscard]]       Mephi::Vector2d& GetRightCorner()        noexcept {return rightCorner_;}
        [[nodiscard]] const sf::Color&       GetFillColor()    const noexcept {return fillColor_;}
        [[nodiscard]]       sf::Color&       GetFillColor()          noexcept {return fillColor_;}
        [[nodiscard]] const sf::Color&       GetOutlineColor() const noexcept {return outlineColor_;}
        [[nodiscard]]       double           Width()           const noexcept {return rightCorner_.x - leftCorner_.x;}
        [[nodiscard]]       double           Height()          const noexcept {return rightCorner_.y - leftCorner_.y;}
};

}
#endif /*MEPHI_REACTOR_SOURCE_FIGURES_INCLUDE_FIGURES_SOLID_RECT_HPP*/
