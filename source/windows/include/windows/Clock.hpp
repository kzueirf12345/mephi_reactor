#ifndef MEPHI_REACTOR_WINDOWS_INCLUDE_WINDOWS_CLOCK_HPP
#define MEPHI_REACTOR_WINDOWS_INCLUDE_WINDOWS_CLOCK_HPP

#include "common/ErrorHandle.hpp"
#include "figures/Rect.hpp"
#include "windows/Window.hpp"

namespace Mephi {

class Clock : public Mephi::Window {
    private:
        static constexpr double SMALL_DOT_RADIUS = 1;
        static constexpr double BIG_DOT_RADIUS = 3;
        sf::Text text_;
        sf::Color arrowColor_;
        sf::Color dotColor_;

        Common::Error UpdateText();

        Common::Error DrawText  (sf::RenderWindow& window) const;
        Common::Error DrawArrows(sf::RenderWindow& window) const;
        Common::Error DrawDots  (sf::RenderWindow& window) const;
        

    public:
        Clock(Mephi::Rect rect, bool isDraggable = true)
            : Mephi::Window{rect, isDraggable}, text_{}, arrowColor_(Common::TNC::ClockHands),
              dotColor_(Common::TNC::ClockDots)
        {
            text_.setFont(Common::GLOBAL_FONT);
            text_.setCharacterSize(30);
            text_.setFillColor(Common::TNC::TextPrimary);
            text_.setStyle(sf::Text::Bold);
        }

        virtual Common::Error Update() override final;
        virtual Common::Error Draw (sf::RenderWindow& window) const override final;
};

}



#endif /* MEPHI_REACTOR_WINDOWS_INCLUDE_WINDOWS_CLOCK_HPP */