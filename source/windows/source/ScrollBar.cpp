#include "windows/ScrollBar.hpp"
#include "common/Constants.hpp"
#include "vector/Vector.hpp"
#include "windows/Window.hpp"
#include "windows/buttons/Button.hpp"
#include <memory>

Mephi::ScrollBar::ScrollBar(Mephi::Rect rect, ActionT action, bool isHorizontal, bool isDraggable)
    : Mephi::Window{std::move(rect), isDraggable}, action_{std::move(action)}, 
      isHorizontal_(std::move(isHorizontal)), percentage_{0}
{

    const Mephi::Vector2d buttonSize (
        ( isHorizontal ? BUTTON_SIZE_PERCENT_ : 1) * rect_.Width(),
        (!isHorizontal ? BUTTON_SIZE_PERCENT_ : 1) * rect_.Height()
    );

    if (isHorizontal) {
        rect_.GetLeftCorner().x += buttonSize.x;
        rect_.GetSize().x -= 2 * buttonSize.x;
    } else {
        rect_.GetLeftCorner().y += buttonSize.y;
        rect_.GetSize().y -= 2 * buttonSize.y;
    }

    std::unique_ptr<Mephi::Window> upButton = std::make_unique<Mephi::Button>(
        Mephi::Rect(
            Mephi::Vector2d(
                ( isHorizontal ? rect_.Width(): 0), 
                (!isHorizontal ? -buttonSize.y : 0)
            ),
            buttonSize
        ), 
        "U"
    ); 
    upButton_ = dynamic_cast<Mephi::Button*>(upButton.get());

    std::unique_ptr<Mephi::Window> downButton = std::make_unique<Mephi::Button>(
        Mephi::Rect(
            Mephi::Vector2d(
                ( isHorizontal ? -buttonSize.x   : 0), 
                (!isHorizontal ? rect_.Height() : 0)
            ),
            buttonSize
        ), 
        "D"
    ); 

    downButton_ = dynamic_cast<Mephi::Button*>(downButton.get());

    std::unique_ptr<Mephi::Window> thumbButton = std::make_unique<Mephi::Button>(
        Mephi::Rect(
            Mephi::Vector2d(0, 0),
            buttonSize
        ), 
        "",
        true
    ); 

    thumbButton_ = dynamic_cast<Mephi::Button*>(thumbButton.get());

    AddChild(std::move(   upButton));
    AddChild(std::move( downButton));
    AddChild(std::move(thumbButton));

    rect_.GetFillColor() = Common::TNC::ScrollbarBackground;
}

// Common::Error Mephi::ScrollBar::Draw(sf::RenderWindow& window) const {
//     ERROR_HANDLE(Mephi::Window::Draw(window));

//     return Common::Error::SUCCESS;
// }