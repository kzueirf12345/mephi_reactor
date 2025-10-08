#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_SCROLL_BAR_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_SCROLL_BAR_HPP

#include "common/ErrorHandle.hpp"
#include "figures/Rect.hpp"
#include "windows/Window.hpp"
#include "windows/buttons/Button.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace Mephi 
{

class ScrollBar: public Mephi::Window {
    public:
        static const sf::Mouse::Button CONTROL_BUTTON_ = sf::Mouse::Button::Left;
        static const sf::Keyboard::Key INC_KEYBOARD_BUTTON_ = sf::Keyboard::Key::Up;
        static const sf::Keyboard::Key DEC_KEYBOARD_BUTTON_ = sf::Keyboard::Key::Down;
        static const sf::Keyboard::Key ZERO_KEYBOARD_BUTTON_ = sf::Keyboard::Key::Numpad2;
        static const sf::Keyboard::Key FULL_KEYBOARD_BUTTON_ = sf::Keyboard::Key::Numpad8;
        using ActionT = std::function<Common::Error(double)>; 

    private:
        static constexpr double BUTTON_SIZE_PERCENT_ = 0.1; 
        static constexpr double MIN_SHIFT_ = 0.05;
    
        ActionT action_;

        double percentage_;
        bool isHorizontal_;
        Mephi::Button* incButton_, *decButton_, *thumbButton_;

        bool isPressedInc_;
        bool isPressedDec_;

        Common::Error Move(double shiftPercent);

    public:
        ScrollBar(Mephi::Rect rect, ActionT action, bool isHorizontal = true, 
                  bool isDraggable = false);

        virtual Common::Error Update() override final;
        virtual bool OnKeyboardPress  (Mephi::EventKeyboardButton event) override final;
        virtual bool OnKeyboardUnpress(Mephi::EventKeyboardButton event) override final;
        virtual bool OnMousePress     (Mephi::EventMouseButton    event) override final;
        virtual bool OnMouseUnpress   (Mephi::EventMouseButton    event) override final;
        virtual bool OnMouseDrag      (Mephi::EventCoord          event) override final;
};

}

#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_SCROLL_BAR_HPP*/