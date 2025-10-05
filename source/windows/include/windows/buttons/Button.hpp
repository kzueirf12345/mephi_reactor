#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_BUTTON_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_BUTTON_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdlib>

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "common/Constants.hpp"
#include "figures/Rect.hpp"
#include "windows/Window.hpp"

namespace Mephi
{

class Button: public Mephi::Window {
    private:
        static constexpr sf::Mouse::Button ACTIVATION_BUTTON_ = sf::Mouse::Left;

    protected:
        sf::Color pressedColor_;
        sf::Color defaultColor_;
        sf::Text  text_;

        bool isPressed_;

    public:
        Button(const Mephi::Rect& rect, 
               const std::string& textString,
               bool isDraggable = false) 
            : Mephi::Window{rect, isDraggable}, defaultColor_(Common::TNC::ButtonIdle), 
              pressedColor_{Common::TNC::ButtonPressed}, text_{}, isPressed_{false}
        {
            text_.setFont(Common::GLOBAL_FONT);
            text_.setString(textString);
            text_.setCharacterSize(14);
            text_.setFillColor(Common::TNC::TextPrimary);
            text_.setStyle(sf::Text::Bold);

            sf::FloatRect textRect = text_.getLocalBounds();
            text_.setPosition(
                (rect_.Width()  - textRect.width)  / 2,
                (rect_.Height() - textRect.height) / 2
            );
        }

        virtual Common::Error Update() override;

        virtual bool OnMousePress  (Mephi::EventMouseButton event) override;
        virtual bool OnMouseUnpress(Mephi::EventMouseButton event) override;
        virtual Common::Error Draw(sf::RenderWindow& window) const override final;

        virtual ~Button() = default;
};

}
#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_BUTTON_HPP*/
