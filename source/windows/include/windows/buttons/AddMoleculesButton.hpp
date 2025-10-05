#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_ADD_MOLECULES_BUTTON_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_ADD_MOLECULES_BUTTON_HPP

#include <cstdlib>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "common/ErrorHandle.hpp"
#include "windows/buttons/Button.hpp"
#include "windows/Reactor.hpp"

namespace Mephi
{

class AddMoleculesButton: public Mephi::Button {
    private:
        Mephi::Reactor* reactor_;
        size_t count_;
        double maxSpeed_;
    public:
        AddMoleculesButton(const Mephi::Rect& rect, const std::string& textString,
                           Mephi::Reactor* reactor = nullptr, size_t count = 0, double maxSpeed = 10,
                           bool isDraggable = false,
                           const sf::Color& pressedColor = sf::Color(139, 0, 0)) 
            : Mephi::Button{rect, textString, isDraggable, pressedColor},
              reactor_{reactor}, count_{count}, maxSpeed_{maxSpeed}
        {}

        virtual Common::Error Update() override final;
};

}
#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_ADD_MOLECULES_BUTTON_HPP*/
