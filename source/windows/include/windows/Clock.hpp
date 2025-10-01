#ifndef MEPHI_REACTOR_WINDOWS_INCLUDE_WINDOWS_CLOCK_HPP
#define MEPHI_REACTOR_WINDOWS_INCLUDE_WINDOWS_CLOCK_HPP

#include "common/ErrorHandle.hpp"
#include "windows/Window.hpp"

namespace Mephi {

class Clock : public Mephi::Window {
    private:
        sf::Text text_;
    public:
        Clock(Mephi::Rect rect, bool isDraggable = true)
            : Mephi::Window{rect, isDraggable}, text_{}
        {
            text_.setFont(Common::GLOBAL_FONT);
            text_.setCharacterSize(34);
            text_.setFillColor(sf::Color::Black);
            text_.setStyle(sf::Text::Bold);
        }

        virtual Common::Error Update() override final;
        virtual Common::Error Draw (sf::RenderWindow& window) const override final;
};

}



#endif /* MEPHI_REACTOR_WINDOWS_INCLUDE_WINDOWS_CLOCK_HPP */