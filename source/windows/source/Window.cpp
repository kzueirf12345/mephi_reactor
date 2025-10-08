#include "windows/Window.hpp"
#include "common/ErrorHandle.hpp"
#include "events/EventCoord.hpp"
#include "events/EventKeyboardButton.hpp"
#include "events/EventMouseButton.hpp"
#include "vector/Vector.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdlib>
#include <memory>
#include <cassert>

Common::Error Mephi::Window::HandleEvents (sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            
            case sf::Event::MouseMoved:
                OnMouseMove(
                    Mephi::EventCoord(Mephi::Vector2d(sf::Mouse::getPosition(window)))
                );

                OnMouseDrag(
                    Mephi::EventCoord(Mephi::Vector2d(sf::Mouse::getPosition(window)))
                );
                
                break;

            case sf::Event::MouseButtonPressed:
                OnMousePress(Mephi::EventMouseButton(
                    Mephi::Vector2d(sf::Mouse::getPosition(window)),
                    event.mouseButton.button
                ));
                
                break;

            case sf::Event::MouseButtonReleased:
                OnMouseUnpress(Mephi::EventMouseButton(
                    Mephi::Vector2d(sf::Mouse::getPosition(window)),
                    event.mouseButton.button
                ));
                break;
            
            case sf::Event::KeyPressed:
                OnKeyboardPress(Mephi::EventKeyboardButton(event.key.code));
                break;

            case sf::Event::KeyReleased:
                OnKeyboardUnpress(Mephi::EventKeyboardButton(event.key.code));
                break;

            default:
                break;
        }
    }

    return Common::Error::SUCCESS;
}

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

Common::Error Mephi::Window::Move(Mephi::Vector2d shift) {
    rect_.GetLeftCorner() += shift;
    return Common::Error::SUCCESS;
}

Common::Error Mephi::Window::SetParent(Mephi::Window* const parent) {
    parent_ = parent;
    return Common::Error::SUCCESS;
}

Common::Error Mephi::Window::UpdateParents_(Mephi::Window* const root) {
    for (auto& child : root->children_) {
        ERROR_HANDLE(child->SetParent(root));
        ERROR_HANDLE(UpdateParents_(child.get()));
    }

    return Common::Error::SUCCESS;
}

Mephi::Window* Mephi::Window::AddChild(std::unique_ptr<Mephi::Window> child) {
    if (child->SetParent(this)) {
        return nullptr;
    }
    children_.push_back(std::move(child));

    if (UpdateParents_(children_.back().get())) {
        return nullptr;
    }

    return children_.back().get();
}

bool Mephi::Window::OnKeyboardPress(Mephi::EventKeyboardButton event) {
    for (auto child = children_.rbegin(); child != children_.rend(); ++child) {
        if ((*child)->OnKeyboardPress(event)) {
            return true;
        }
    }

    return false;
}

bool Mephi::Window::OnKeyboardUnpress(Mephi::EventKeyboardButton event) {
    for (auto child = children_.rbegin(); child != children_.rend(); ++child) {
        if ((*child)->OnKeyboardUnpress(event)) {
            return true;
        }
    }

    return false;
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
            auto it = child.base();
            std::rotate(it - 1, it, children_.end());
            
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
        if (parent_) {
            rect_.GetLeftCorner() = rect_.GetLeftCorner().Clump(
                {0, 0}, 
                parent_->GetRect().GetSize() - rect_.GetSize()
            );
        }
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