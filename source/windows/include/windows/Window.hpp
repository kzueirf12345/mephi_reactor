#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_WINDOW_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_WINDOW_HPP

#include "common/ErrorHandle.hpp"
#include "figures/Rect.hpp"
#include "vector/Vector.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

namespace Mephi
{

class Window {
    private:
        static constexpr sf::Mouse::Button MOVE_BUTTON_ = sf::Mouse::Button::Middle;

    protected:
        Mephi::Rect rect_;
        std::vector<Mephi::Window*> children_;
        Mephi::Window* parent_;

        [[nodiscard]] Mephi::Vector2d AbsoluteCoord() const noexcept;
    public:
        Window(Mephi::Rect rect, Mephi::Window* const parent = nullptr)
            : rect_{std::move(rect)}, parent_(parent)
        {}

        virtual Common::Error Draw(sf::RenderWindow& window) const;
};


}
#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_WINDOW_HPP*/
