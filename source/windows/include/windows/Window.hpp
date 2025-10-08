#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_WINDOW_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_WINDOW_HPP

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

#include "common/ErrorHandle.hpp"
#include "events/EventMouseButton.hpp"
#include "figures/Rect.hpp"
#include "vector/Vector.hpp"
#include "events/EventCoord.hpp"


namespace Mephi
{

class Window {
    private:

    protected:
        Mephi::Rect rect_;
        std::vector<std::unique_ptr<Mephi::Window>> children_;
        Mephi::Window* parent_;

        bool isDraggable_;
        
        bool isSelected_;
        bool isHovered_;
        bool isInderectHovered_;
        
        Mephi::Vector2d prevMousePos_;

        Common::Error UpdateParents_(Mephi::Window* const root);

        [[nodiscard]] Mephi::Vector2d AbsoluteCoord() const noexcept;

    public:
        explicit Window(Mephi::Rect rect, bool isDraggable = true)
            : rect_{std::move(rect)}, parent_(nullptr), children_{}, 
              isHovered_{false}, isInderectHovered_{false}, isDraggable_{isDraggable}, 
              prevMousePos_{{0, 0}}, isSelected_{false}
        {}

        Common::Error SetParent(Mephi::Window* const parent);
        Mephi::Window* AddChild(std::unique_ptr<Mephi::Window> child);

        virtual Common::Error Draw  (sf::RenderWindow& window) const;
        virtual Common::Error Update();


        Common::Error HandleEvents (sf::RenderWindow& window);
        virtual bool OnMouseMove   (Mephi::EventCoord       event);
        virtual bool OnMousePress  (Mephi::EventMouseButton event);
        virtual bool OnMouseUnpress(Mephi::EventMouseButton event);
        virtual bool OnMouseDrag   (Mephi::EventCoord       event);

        Window& operator [](size_t ind) { return *children_[ind].get(); } 

        const Window& operator [](size_t ind) const { return *children_[ind].get(); } 

        [[nodiscard]] bool                 IsHovered()          const noexcept { return         isHovered_; }
        [[nodiscard]] bool                 IsInderectHovered()  const noexcept { return isInderectHovered_; }
        [[nodiscard]] bool                 isSelected()         const noexcept { return         isSelected_; }
        [[nodiscard]] const sf::Color&     GetFillColor()       const noexcept {return rect_.GetFillColor();}
        [[nodiscard]]       sf::Color&     GetFillColor()             noexcept {return rect_.GetFillColor();}
        [[nodiscard]] const sf::Color&     GetOutlineColor()    const noexcept {return rect_.GetOutlineColor();}
        [[nodiscard]]       sf::Color&     GetOutlineColor()          noexcept {return rect_.GetOutlineColor();}
        [[nodiscard]] const Mephi::Rect&   GetRect()            const noexcept {return rect_;}
        [[nodiscard]]       Mephi::Window* GetLastChild()       noexcept       {return children_.back().get();}

        virtual ~Window() = default;
};


}
#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_WINDOW_HPP*/
