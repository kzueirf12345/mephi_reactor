#include "windows/Window.hpp"
#include "common/ErrorHandle.hpp"
#include "events/EventCoord.hpp"
#include "events/EventMouseButton.hpp"
#include "vector/Vector.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdlib>
#include <memory>

Mephi::Vector2d Mephi::Window::AbsoluteCoord() const noexcept {
    if (parent_ == nullptr)
        return rect_.GetLeftCorner();

    return parent_->AbsoluteCoord() + rect_.GetLeftCorner();
}

Common::Error Mephi::Window::Draw(sf::RenderWindow& window) const {
    sf::RectangleShape sfRect(rect_.GetSFRect());
    sfRect.setPosition(static_cast<sf::Vector2f>(AbsoluteCoord()));

    window.draw(sfRect);

    for (const auto& child : children_) {
        ERROR_HANDLE(child->Draw(window));
    }

    return Common::Error::SUCCESS;
}

Common::Error Mephi::Window::Update() {
    for (auto& child : children_) {
        ERROR_HANDLE(child->Update());
    }

    return Common::Error::SUCCESS;
}

Common::Error Mephi::Window::AddChild(std::unique_ptr<Mephi::Window> child) {
    children_.push_back(std::move(child));

    return Common::Error::SUCCESS;
}

bool Mephi::Window::OnMouseMove(Mephi::EventCoord event) {
    isInderectHovered_ = rect_.OnMe(event.coord);

    for (auto child = children_.rbegin(); child != children_.rend(); ++child) {
        if ((*child)->OnMouseMove(Mephi::EventCoord(event.coord - rect_.GetLeftCorner()))) {
            return true;
        }
    }

    isHovered_ = isInderectHovered_;

    return isHovered_;
}

bool Mephi::Window::OnMouseUnpress(Mephi::EventMouseButton event) {
    Mephi::EventMouseButton childEvent(event);
    childEvent.coord -= rect_.GetLeftCorner();
    for (auto child = children_.rbegin(); child != children_.rend(); ++child) {
        if ((*child)->OnMouseUnpress(childEvent)) {
            return true;
        }
    }

    if (isSelected_ && event.button == Mephi::EventMouseButton::MOVE_BUTTON_) {
        isSelected_ = false;
        return true;
    }

    return false;
}

bool Mephi::Window::OnMousePress(Mephi::EventMouseButton event) {
    Mephi::EventMouseButton childEvent(event);
    childEvent.coord -= rect_.GetLeftCorner();
    for (auto child = children_.rbegin(); child != children_.rend(); ++child) {
        if ((*child)->OnMousePress(childEvent)) {
            isSelected_ = false;
            return true;
        }
    }

    if (isDraggable_ 
     && isInderectHovered_
     && event.button == Mephi::EventMouseButton::MOVE_BUTTON_) {
        isSelected_ = true;
        prevMousePos_ = event.coord;
        return true;
    }

    return false;
}

bool Mephi::Window::OnMouseDrag(Mephi::EventCoord event) {
    if (isDraggable_ && isSelected_) {
        rect_.GetLeftCorner() += event.coord - prevMousePos_;
        prevMousePos_ = event.coord;
        return true;
    }

    Mephi::EventCoord childEvent(event);
    childEvent.coord -= rect_.GetLeftCorner();
    for (auto child = children_.rbegin(); child != children_.rend(); ++child) {
        if ((*child)->OnMouseDrag(childEvent)) {
            return true;
        }
    }

    return false;
}