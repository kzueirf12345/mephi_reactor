#ifndef MEPHI_REACTOR_WINDOWS_INCLUDE_WINDOWS_CLOCK_HPP
#define MEPHI_REACTOR_WINDOWS_INCLUDE_WINDOWS_CLOCK_HPP

#include <chrono>

#include "common/ErrorHandle.hpp"
#include "windows/Window.hpp"

namespace Mephi {

class Clock : public Mephi::Window {
    private:
        static constexpr size_t TIME_STRING_SIZE_ = 80;
        sf::Text text_;
    public:
        Clock(Mephi::Rect rect, bool isDraggable = true)
            : Mephi::Window{rect, isDraggable}, text_{}
        {
            text_.setFont(Common::GLOBAL_FONT);
            text_.setCharacterSize(24);
            text_.setFillColor(sf::Color::Black);
            text_.setStyle(sf::Text::Bold);

        }

        virtual Common::Error Update() {
            char timeString_[TIME_STRING_SIZE_];
            auto now = std::chrono::system_clock::now();

            std::time_t now_c = std::chrono::system_clock::to_time_t(now);

            std::tm* now_tm = std::localtime(&now_c);

            std::strftime(timeString_, TIME_STRING_SIZE_, "%H:%M:%S", now_tm);
            text_.setString(timeString_);

            sf::FloatRect textRect = text_.getLocalBounds();
            text_.setPosition(
                (rect_.Width()  - textRect.width)  / 2,
                (rect_.Height() - textRect.height) / 2
            );
            return Common::Error::SUCCESS;
        }
        
        virtual Common::Error Draw (sf::RenderWindow& window) const {
            Mephi::Window::Draw(window);

            sf::Text printText = text_;
            printText.setPosition(
                static_cast<sf::Vector2f>(AbsoluteCoord()) 
              + printText.getPosition() 
            );

            window.draw(printText);
            return Common::Error::SUCCESS;
        }
};

}



#endif /* MEPHI_REACTOR_WINDOWS_INCLUDE_WINDOWS_CLOCK_HPP */