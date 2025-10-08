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

Common::Error Mephi::ScrollBar::Move(double shiftPercent) {
    const double newPercentage = std::clamp(percentage_ + shiftPercent, 0., 1.);
    const double realShift = newPercentage - percentage_;
    percentage_ = newPercentage;

    ERROR_HANDLE(action_(percentage_));
    
    ERROR_HANDLE(thumbButton_->Move(Mephi::Vector2d(
        ( isHorizontal_ ?  realShift : 0) * (rect_.Width()  - thumbButton_->GetRect().Width()),
        (!isHorizontal_ ? -realShift : 0) * (rect_.Height() - thumbButton_->GetRect().Height())
    )));

    return Common::Error::SUCCESS;
}

Common::Error Mephi::ScrollBar::Update() {
    if (incButton_->IsPressed()) {
        ERROR_HANDLE(Move(MIN_SHIFT_));
    }

    if (decButton_->IsPressed()) {
        ERROR_HANDLE(Move(-MIN_SHIFT_));
    }

    ERROR_HANDLE(Mephi::Window::Update());

    return Common::Error::SUCCESS;
}

// bool Mephi::ScrollBar::OnKeyboardPress(Mephi::EventKeyboardButton event) {

// }

bool Mephi::ScrollBar::OnMousePress(Mephi::EventMouseButton event) {
    if (event.button == Mephi::EventMouseButton::MOVE_BUTTON_) {
        return false;
    }

    return Mephi::Window::OnMousePress(event);
}

bool Mephi::ScrollBar::OnMouseUnpress(Mephi::EventMouseButton event) {
    if (event.button == Mephi::EventMouseButton::MOVE_BUTTON_) {
        return false;
    }

    return Mephi::Window::OnMouseUnpress(event);
}

bool Mephi::ScrollBar::OnMouseDrag(Mephi::EventCoord event) {
    if (thumbButton_->IsPressed()) {
        const double curThumbCoord = (
            isHorizontal_ 
          ? event.coord.x - rect_.GetLeftCorner().x 
          : rect_.GetRightCorner().y - event.coord.y
        );
        const double maxThumbCoord = (
            isHorizontal_ 
          ? rect_.Width()  - thumbButton_->GetRect().Width()
          : rect_.Height() - thumbButton_->GetRect().Height()
        );

        const double curPercentage = curThumbCoord / maxThumbCoord;
        const double shiftPercent = curPercentage - percentage_;

        ERROR_HANDLE(Move(shiftPercent));

        return true;
    }

    return Mephi::Window::OnMouseDrag(event);
}

