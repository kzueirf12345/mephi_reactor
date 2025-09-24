#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_BUTTON_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_BUTTON_HPP

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
        sf::Color pressedColor_;
        sf::Color defaultColor_;

    public:
        explicit Button(const Mephi::Rect& rect, 
                        const sf::Color& defaultColor = sf::Color(220, 20, 60),
                        const sf::Color& pressedColor = sf::Color(139, 0, 0)) 
            : Mephi::Window{rect}, defaultColor_(defaultColor), pressedColor_{pressedColor}
        {}

        virtual Common::Error HandlePressed(const Mephi::Vector2i& mousePos);
};

}
#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_BUTTON_HPP*/
