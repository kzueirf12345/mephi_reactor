#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_SCROLL_BAR_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_SCROLL_BAR_HPP

#include "figures/Rect.hpp"
#include "vector/Vector.hpp"
#include "windows/Window.hpp"
#include "windows/buttons/Button.hpp"

namespace Mephi 
{

class ScrollBar: public Mephi::Window {
    public:
        using ActionT = std::function<Common::Error(int)>; 

    private:
        static constexpr double BUTTON_SIZE_PERCENT_ = 0.1; 
    
        ActionT action_;

        unsigned int percentage_;
        bool isHorizontal_;
        Mephi::Button* upButton_, *downButton_, *thumbButton_;

    public:
        ScrollBar(Mephi::Rect rect, ActionT action, bool isHorizontal = true, 
                  bool isDraggable = false);

        // virtual Common::Error Draw(sf::RenderWindow& window) const override final;
};

}

#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_SCROLL_BAR_HPP*/