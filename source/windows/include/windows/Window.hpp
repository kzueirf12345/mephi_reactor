#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_WINDOW_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_WINDOW_HPP

#include "common/ErrorHandle.hpp"
#include "figures/Rect.hpp"
#include "vector/Vector.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include <memory>
namespace Mephi
{

class Window {
    private:
        static constexpr sf::Mouse::Button MOVE_BUTTON_ = sf::Mouse::Button::Middle;

    protected:
        Mephi::Rect rect_;
        std::vector<std::unique_ptr<Mephi::Window>> children_;
        Mephi::Window* parent_;

        [[nodiscard]] Mephi::Vector2d AbsoluteCoord() const noexcept;
    public:
        explicit Window(Mephi::Rect rect, Mephi::Window* const parent = nullptr)
            : rect_{std::move(rect)}, parent_(parent)
        {}

        Common::Error AddChild(std::unique_ptr<Mephi::Window> child);

        virtual Common::Error Draw  (sf::RenderWindow& window) const;
        virtual Common::Error Update();

        Window& operator [](size_t ind) { return *children_[ind].get(); } 

        const Window& operator [](size_t ind) const { return *children_[ind].get(); } 

        virtual ~Window() = default;
};


}
#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_WINDOW_HPP*/
