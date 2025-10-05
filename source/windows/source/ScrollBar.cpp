#include "windows/ScrollBar.hpp"
#include "common/Constants.hpp"
#include "common/ErrorHandle.hpp"
#include "windows/Window.hpp"
#include "windows/buttons/Button.hpp"
#include <memory>

Mephi::ScrollBar::ScrollBar(Mephi::Rect rect, ActionT action, bool isHorizontal, bool isDraggable)
    : Mephi::Window{std::move(rect), isDraggable}, action_{std::move(action)}, 
      isHorizontal_(std::move(isHorizontal)), percentage_{0}
{
    std::unique_ptr<Mephi::Window> upButton = std::make_unique<Mephi::Button>(
        Mephi::Rect(
            Mephi::Vector2d(0, 0),
            Mephi::Vector2d(
                ( isHorizontal ? BUTTON_SIZE_PERCENT_ : 1) * rect.Width(),
                (!isHorizontal ? BUTTON_SIZE_PERCENT_ : 1) * rect.Height()
            )
        ), 
        "U"
    ); 
    upButton_ = dynamic_cast<Mephi::Button*>(upButton.get());

    std::unique_ptr<Mephi::Window> downButton = std::make_unique<Mephi::Button>(
        Mephi::Rect(
            Mephi::Vector2d(
                ( isHorizontal ? 1 - BUTTON_SIZE_PERCENT_ : 0) * rect.Width(), 
                (!isHorizontal ? 1 - BUTTON_SIZE_PERCENT_ : 0) * rect.Height()
            ),
            Mephi::Vector2d(
                ( isHorizontal ? BUTTON_SIZE_PERCENT_ : 1) * rect.Width(),
                (!isHorizontal ? BUTTON_SIZE_PERCENT_ : 1) * rect.Height()
            )
        ), 
        "D"
    ); 

    downButton_ = dynamic_cast<Mephi::Button*>(downButton.get());

    std::unique_ptr<Mephi::Window> thumbButton = std::make_unique<Mephi::Button>(
        Mephi::Rect(
            Mephi::Vector2d(
                ( isHorizontal ? BUTTON_SIZE_PERCENT_ : 0) * rect.Width(), 
                (!isHorizontal ? BUTTON_SIZE_PERCENT_ : 0) * rect.Height()
            ),
            Mephi::Vector2d(
                ( isHorizontal ? BUTTON_SIZE_PERCENT_ : 1) * rect.Width(),
                (!isHorizontal ? BUTTON_SIZE_PERCENT_ : 1) * rect.Height()
            )
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