#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_WALL_TEMP_BUTTON_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_WALL_TEMP_BUTTON_HPP

#include <chrono>
#include <cstdlib>

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <functional>

#include "windows/buttons/Button.hpp"

namespace Mephi
{

template <typename T = double>
class AdjustButton: public Mephi::Button {
    private:
        static constexpr sf::Mouse::Button INCREASE_BUTTON_ = sf::Mouse::Button::Left;
        static constexpr sf::Mouse::Button DECREASE_BUTTON_ = sf::Mouse::Button::Right;

                               T  changeRate_;
        std::reference_wrapper<T> obj_;
        std::chrono::time_point<std::chrono::steady_clock> prevTime_;

    public:
        AdjustButton(const Mephi::Rect& rect, T& obj, T changeRate,
                     const sf::Color& defaultColor = sf::Color(220, 20, 60),
                     const sf::Color& pressedColor = sf::Color(139, 0, 0)) 
            : Mephi::Button{rect, defaultColor, pressedColor}, obj_{obj}, changeRate_(std::move(changeRate)),
              prevTime_{std::chrono::milliseconds(0)}
        {}

        virtual Common::Error HandlePressed(const Mephi::Vector2i& mousePos) override final;

        // [[nodiscard]] T GetT() noexcept {return obj_.get();}
};

template<typename T>
Common::Error Mephi::AdjustButton<T>::HandlePressed(const Mephi::Vector2i& mousePos) {
    const bool increased = CheckPressed(mousePos, INCREASE_BUTTON_);
    const bool decreased = CheckPressed(mousePos, DECREASE_BUTTON_);

    if (increased || decreased) {
        const auto curTime = std::chrono::steady_clock::now();
        const auto durTime = curTime - (prevTime_.time_since_epoch() == std::chrono::milliseconds(0) 
                                        ? std::chrono::steady_clock::now() 
                                        : prevTime_
        );
        prevTime_= curTime;
        const double durTimeS = durTime.count() / 1e8;

        obj_.get() += durTimeS * (increased ? changeRate_ : -changeRate_);
        rect_.GetFillColor() = pressedColor_;
    } else {
        prevTime_ = std::chrono::time_point<std::chrono::steady_clock>(std::chrono::milliseconds(0));
        rect_.GetFillColor() = defaultColor_;
    }

    return Common::Error::SUCCESS;
}

}
#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_WALL_TEMP_BUTTON_HPP*/
