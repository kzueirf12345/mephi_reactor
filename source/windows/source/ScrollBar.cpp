#include "windows/ScrollBar.hpp"
#include "common/Constants.hpp"
#include "common/ErrorHandle.hpp"
#include "events/EventMouseButton.hpp"
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

    std::unique_ptr<Mephi::Window> incButton = std::make_unique<Mephi::Button>(
        Mephi::Rect(
            Mephi::Vector2d(
                ( isHorizontal ? rect_.Width(): 0), 
                (!isHorizontal ? -buttonSize.y : 0)
            ),
            buttonSize
        ), 
        "+"
    ); 
    incButton_ = dynamic_cast<Mephi::Button*>(incButton.get());

    std::unique_ptr<Mephi::Window> decButton = std::make_unique<Mephi::Button>(
        Mephi::Rect(
            Mephi::Vector2d(
                ( isHorizontal ? -buttonSize.x   : 0), 
                (!isHorizontal ? rect_.Height() : 0)
            ),
            buttonSize
        ), 
        "-"
    ); 

    decButton_ = dynamic_cast<Mephi::Button*>(decButton.get());

    std::unique_ptr<Mephi::Window> thumbButton = std::make_unique<Mephi::Button>(
        Mephi::Rect(
            Mephi::Vector2d(
                0, 
                (!isHorizontal ? rect_.Height() - buttonSize.y: 0)
            ),
            buttonSize
        ), 
        "",
        true
    ); 

    thumbButton_ = dynamic_cast<Mephi::Button*>(thumbButton.get());

    AddChild(std::move(   incButton));
    AddChild(std::move( decButton));
    AddChild(std::move(thumbButton));

    rect_.GetFillColor() = Common::TNC::ScrollbarBackground;
}

bool Mephi::ScrollBar::OnMousePress(Mephi::EventMouseButton event) {

    Mephi::EventMouseButton childEvent(event);
    childEvent.coord -= rect_.GetLeftCorner();

    if (event.button == THUMB_DRAG_BUTTON_) {
        childEvent.button = Mephi::EventMouseButton::MOVE_BUTTON_;
        if (thumbButton_->OnMousePress(childEvent)) {
            return true;
        }
    }

    if (event.button == Mephi::EventMouseButton::MOVE_BUTTON_) {
        return false;
    }

    return Mephi::Window::OnMousePress(event);
}

bool Mephi::ScrollBar::OnMouseUnpress(Mephi::EventMouseButton event) {
    Mephi::EventMouseButton childEvent(event);
    childEvent.coord -= rect_.GetLeftCorner();

    if (event.button == THUMB_DRAG_BUTTON_) {
        childEvent.button = Mephi::EventMouseButton::MOVE_BUTTON_;
        if (thumbButton_->OnMouseUnpress(childEvent)) {
            return true;
        }
    } 

    if (event.button == Mephi::EventMouseButton::MOVE_BUTTON_) {
        return false;
    }

    return Mephi::Window::OnMouseUnpress(event);
}

bool Mephi::ScrollBar::OnMouseDrag(Mephi::EventCoord event) {
    if (Mephi::Window::OnMouseDrag(event)) {
        const double curThumbCoord = (
            isHorizontal_ 
          ? thumbButton_->GetRect().GetLeftCorner().x 
          : thumbButton_->GetRect().GetLeftCorner().y
        );
        const double maxThumbCoord = (
            isHorizontal_ 
          ? rect_.Width()  - thumbButton_->GetRect().Width()
          : rect_.Height() - thumbButton_->GetRect().Height()
        );
        percentage_ = curThumbCoord / maxThumbCoord;
        if (!isHorizontal_) {
            percentage_ = 1 - percentage_;
        }
        ERROR_HANDLE(action_(percentage_));
        return true;
    }

    return false;
}

