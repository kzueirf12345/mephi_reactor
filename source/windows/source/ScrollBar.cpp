#include "windows/ScrollBar.hpp"
#include "common/Constants.hpp"
#include "common/ErrorHandle.hpp"
#include "events/EventMouseButton.hpp"
#include "vector/Vector.hpp"
#include "windows/Window.hpp"
#include "windows/buttons/Button.hpp"
#include <memory>
#include <cassert>

Mephi::ScrollBar::ScrollBar(Mephi::Rect rect, ActionT action, bool isHorizontal, bool isDraggable)
    : Mephi::Window{std::move(rect), isDraggable}, action_{std::move(action)}, 
      isHorizontal_(std::move(isHorizontal)), percentage_{0}, isPressedInc_{false}, isPressedDec_{false}
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

    // std::cerr << "inc " << isPressedInc_ << " dec " << isPressedDec_ << std::endl;
    if (incButton_->IsPressed() || isPressedInc_) {
        ERROR_HANDLE(Move(MIN_SHIFT_));
    }
    else if (decButton_->IsPressed() || isPressedDec_) {
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

    if (Mephi::Window::OnMousePress(event)) {
        return true;
    }

    if (isHovered_ && event.button == CONTROL_BUTTON_) {
        if (isHorizontal_) {
            if (event.coord.x < thumbButton_->GetRect().GetLeftCorner().x) {
                isPressedDec_ = true;
            } else {
                isPressedInc_ = true;
            }
        } else {
            if (event.coord.y > thumbButton_->GetRect().GetRightCorner().y) {
                isPressedDec_ = true;
            } else {
                isPressedInc_ = true;
            }
        }

        return true;
    }

    return false;
}

bool Mephi::ScrollBar::OnMouseUnpress(Mephi::EventMouseButton event) {
    if (event.button == Mephi::EventMouseButton::MOVE_BUTTON_) {
        return false;
    }

    if (Mephi::Window::OnMouseUnpress(event)) {
        return true;
    }

    if (event.button == CONTROL_BUTTON_) {
        isPressedDec_ = false;
        isPressedInc_ = false;
        return true;
    }

    return false;
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

