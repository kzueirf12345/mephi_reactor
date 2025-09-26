#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_BUTTON_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_BUTTON_HPP

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdlib>

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "windows/Window.hpp"

namespace Mephi
{

class Button: public Mephi::Window {
    protected:
        const 
        sf::Color pressedColor_;
        sf::Color defaultColor_;
        sf::Text  text_;

    public:
        explicit Button(const Mephi::Rect& rect, 
                        const std::string& textString,
                        const sf::Color& defaultColor = sf::Color(220, 20, 60),
                        const sf::Color& pressedColor = sf::Color(139, 0, 0)) 
            : Mephi::Window{rect}, defaultColor_(defaultColor), pressedColor_{pressedColor}, text_{}
        {
            text_.setFont(Common::GLOBAL_FONT);
            text_.setString(textString);
            text_.setCharacterSize(14);
            text_.setFillColor(sf::Color::Black);
            text_.setStyle(sf::Text::Bold);
            text_.setPosition(static_cast<sf::Vector2f>(rect.GetLeftCorner()));
        }

        virtual Common::Error Move(const Mephi::Vector2d& shift) override final;

        virtual Common::Error HandlePressed(const Mephi::Vector2i& mousePos) override;

        virtual Common::Error Draw(sf::RenderWindow& window) override;
};

}
#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_BUTTON_HPP*/
