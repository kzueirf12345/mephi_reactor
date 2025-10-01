#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_WINDOW_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_WINDOW_HPP

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

#include "common/ErrorHandle.hpp"
#include "figures/Rect.hpp"
#include "vector/Vector.hpp"
#include "events/EventCoord.hpp"


namespace Mephi
{

class Window {
    private:
        static constexpr sf::Mouse::Button MOVE_BUTTON_ = sf::Mouse::Button::Middle;

    protected:
        Mephi::Rect rect_;
        std::vector<std::unique_ptr<Mephi::Window>> children_;
        Mephi::Window* parent_;

        bool isHovered_;
        bool isInderectHovered_;

        [[nodiscard]] Mephi::Vector2d AbsoluteCoord() const noexcept;
    public:
        explicit Window(Mephi::Rect rect, Mephi::Window* const parent = nullptr)
            : rect_{std::move(rect)}, parent_(parent), children_{}, 
              isHovered_{false}, isInderectHovered_{false}
        {}

        Common::Error AddChild(std::unique_ptr<Mephi::Window> child);

        virtual Common::Error Draw  (sf::RenderWindow& window) const;
        virtual Common::Error Update();

        virtual bool OnMouseMove(Mephi::EventCoord event);

        Window& operator [](size_t ind) { return *children_[ind].get(); } 

        const Window& operator [](size_t ind) const { return *children_[ind].get(); } 

        [[nodiscard]] bool         IsHovered() const noexcept { return         isHovered_; }
        [[nodiscard]] bool IsInderectHovered() const noexcept { return isInderectHovered_; }

        virtual ~Window() = default;
};


}
#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_WINDOW_HPP*/
