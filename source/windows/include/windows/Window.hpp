#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_WINDOW_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_WINDOW_HPP

#include "figures/Rect.hpp"
#include "vector/Vector.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include <memory>

namespace Mephi
{

class Window{
    protected:
        static constexpr sf::Mouse::Button MOVE_BUTTON_ = sf::Mouse::Button::Middle;

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

        [[nodiscard]] bool CheckPressed(const Mephi::Vector2i& mousePos, const sf::Mouse::Button& mouseButton = MOVE_BUTTON_) const;
                      bool CheckHold   (const Mephi::Vector2i& mousePos, const sf::Mouse::Button& mouseButton = MOVE_BUTTON_);
        [[nodiscard]] Mephi::Vector2i HandleMouseShift(const Mephi::Vector2i& curMousePos);
        virtual Common::Error Move(const Mephi::Vector2d& shift);

        Mephi::Vector2i HandleDrag(const Mephi::Vector2i& curMousePos);

        [[nodiscard]] const Mephi::Rect& GetRect() const noexcept { return rect_; }

        virtual Common::Error Draw(sf::RenderWindow& window);
        virtual Common::Error HandlePressed(const Mephi::Vector2i& mousePos);

        Common::Error AddChild(std::unique_ptr<Window> child);
        
        [[nodiscard]] const std::vector<std::unique_ptr<Window>>& GetChildren() {return children_;}
};


}
#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_WINDOW_HPP*/
