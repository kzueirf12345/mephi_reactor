#ifndef MEPHI_REACTOR_WINDOWS_INCLUDE_WINDOWS_CLOCK_HPP
#define MEPHI_REACTOR_WINDOWS_INCLUDE_WINDOWS_CLOCK_HPP

#include "common/ErrorHandle.hpp"
#include "windows/Window.hpp"

namespace Mephi {

class Clock : public Mephi::Window {
    private:
        static constexpr double SMALL_DOT_RADIUS = 1;
        static constexpr double BIG_DOT_RADIUS = 3;
        sf::Text text_;
        sf::Color arrowColor_;
        sf::Color dotColor_;

        Common::Error DrawArrows(sf::RenderWindow& window) const;
        Common::Error DrawDots  (sf::RenderWindow& window) const;

    public:
        Clock(Mephi::Rect rect, bool isDraggable = true)
            : Mephi::Window{rect, isDraggable}, text_{}, arrowColor_(sf::Color(0xc0, 0xca, 0xf5)),
              dotColor_(0xbb, 0x9a, 0xf7)
        {
            text_.setFont(Common::GLOBAL_FONT);
            text_.setCharacterSize(30);
            text_.setFillColor(sf::Color(0xa9, 0xb1, 0xd6));
            text_.setStyle(sf::Text::Bold);
        }

        virtual Common::Error Update() override final;
        virtual Common::Error Draw (sf::RenderWindow& window) const override final;
};

}



#endif /* MEPHI_REACTOR_WINDOWS_INCLUDE_WINDOWS_CLOCK_HPP */