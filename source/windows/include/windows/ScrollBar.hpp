#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_SCROLL_BAR_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_SCROLL_BAR_HPP

#include "common/ErrorHandle.hpp"
#include "figures/Rect.hpp"
#include "vector/Vector.hpp"
#include "windows/Window.hpp"
#include "windows/buttons/Button.hpp"
#include <SFML/Window/Mouse.hpp>

namespace Mephi 
{

class ScrollBar: public Mephi::Window {
    public:
        static const sf::Mouse::Button THUMB_DRAG_BUTTON_ = sf::Mouse::Button::Left;
        using ActionT = std::function<Common::Error(double)>; 

    private:
        static constexpr double BUTTON_SIZE_PERCENT_ = 0.1; 
    
        ActionT action_;

        double percentage_;
        bool isHorizontal_;
        Mephi::Button* incButton_, *decButton_, *thumbButton_;

    public:
        ScrollBar(Mephi::Rect rect, ActionT action, bool isHorizontal = true, 
                  bool isDraggable = false);

        virtual bool OnMousePress   (Mephi::EventMouseButton event) override final;
        virtual bool OnMouseUnpress (Mephi::EventMouseButton event) override final;
        virtual bool OnMouseDrag    (Mephi::EventCoord       event) override final;
};

}

#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_SCROLL_BAR_HPP*/