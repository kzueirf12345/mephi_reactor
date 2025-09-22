#ifndef MEPHI_REACTOR_SOURCE_WINDOW_INCLUDE_WINDOW_WINDOW_HPP
#define MEPHI_REACTOR_SOURCE_WINDOW_INCLUDE_WINDOW_WINDOW_HPP

#include "figures/Rect.hpp"
#include "vector/Vector.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

namespace Mephi
{

class Window{
    protected:
        static constexpr sf::Mouse::Button MOVE_BUTTON_ = sf::Mouse::Button::Left;

        Mephi::Rect rect_;
        bool isHold_;
        Mephi::Vector2i prevMousePos_;
        std::vector<std::unique_ptr<Window>> children_;
    public:
        static constexpr int POISON_MOUSE_POS_VAL_ = 228666133;
        inline static const Mephi::Vector2i POISON_MOUSE_POS_{
            Mephi::Window::POISON_MOUSE_POS_VAL_, 
            Mephi::Window::POISON_MOUSE_POS_VAL_
        };

        Window(const Mephi::Rect& rect)
            : rect_{rect}, isHold_{false}, prevMousePos_{POISON_MOUSE_POS_}, children_{}
        {}

        [[nodiscard]] bool CheckPressed(const Mephi::Vector2i& mousePos) const;
                      bool CheckHold   (const Mephi::Vector2i& mousePos);
        [[nodiscard]] Mephi::Vector2i HandleMouseShift(const Mephi::Vector2i& curMousePos);
        Common::Error Move(const Mephi::Vector2d& shift);

        Mephi::Vector2i HandleDrag(const Mephi::Vector2i& curMousePos);

        [[nodiscard]] const Mephi::Rect& GetRect() const noexcept { return rect_; }

        virtual Common::Error Draw(sf::RenderWindow& window) const = 0;
};


}
#endif /*MEPHI_REACTOR_SOURCE_WINDOW_INCLUDE_WINDOW_WINDOW_HPP*/
