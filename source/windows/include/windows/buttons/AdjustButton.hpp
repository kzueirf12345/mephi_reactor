#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_WALL_TEMP_BUTTON_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_WALL_TEMP_BUTTON_HPP

#include <chrono>
#include <cstdlib>

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "common/ErrorHandle.hpp"
#include "windows/buttons/Button.hpp"

namespace Mephi
{

template <typename T>
class AdjustButton: public Mephi::Button {
    private:
        static constexpr sf::Mouse::Button INCREASE_BUTTON_ = sf::Mouse::Button::Left;
        static constexpr sf::Mouse::Button DECREASE_BUTTON_ = sf::Mouse::Button::Right;

        T  changeRate_;
        T* obj_;
        std::chrono::time_point<std::chrono::steady_clock> prevTime_;

    public:
        AdjustButton(const Mephi::Rect& rect, T* const obj, T changeRate, const std::string& textString,
                     bool isDraggable = false,
                     const sf::Color& pressedColor = sf::Color(139, 0, 0)) 
            : Mephi::Button{rect, textString, isDraggable, pressedColor}, obj_{obj}, 
              changeRate_(std::move(changeRate)), prevTime_{std::chrono::steady_clock::duration::zero()}
        {}

        virtual Common::Error Update() override final;
        virtual bool OnMousePress  (Mephi::EventMouseButton event) override final;
        virtual bool OnMouseUnpress(Mephi::EventMouseButton event) override final;
};

template<typename T>
bool Mephi::AdjustButton<T>::OnMousePress(Mephi::EventMouseButton event) {
    const bool increased = (event.button == INCREASE_BUTTON_);
    const bool decreased = (event.button == DECREASE_BUTTON_);

    if (isHovered_ && (increased || decreased)) {
        isPressed_ = true;
        prevTime_ = std::chrono::steady_clock::now();
    }


    return Mephi::Window::OnMousePress(event);
}

template<typename T>
bool Mephi::AdjustButton<T>::OnMouseUnpress(Mephi::EventMouseButton event) {
    const bool increased = (event.button == INCREASE_BUTTON_);
    const bool decreased = (event.button == DECREASE_BUTTON_);

    if (increased || decreased) {
        isPressed_ = false;
    }

    return Mephi::Window::OnMouseUnpress(event);
}

template<typename T>
Common::Error Mephi::AdjustButton<T>::Update() {

    if (isPressed_) {
        const auto curTime = std::chrono::steady_clock::now();
        const auto durTime = curTime - (prevTime_.time_since_epoch() == std::chrono::steady_clock::duration::zero() 
                                        ? std::chrono::steady_clock::now() 
                                        : prevTime_
        );
        prevTime_= curTime;
        const double durTimeS = std::chrono::duration<double>(durTime).count();

        *obj_ += durTimeS * (sf::Mouse::isButtonPressed(INCREASE_BUTTON_) ? changeRate_ : -changeRate_);
    }

    ERROR_HANDLE(Mephi::Button::Update());

    return Common::Error::SUCCESS;
}

}
#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_WALL_TEMP_BUTTON_HPP*/
