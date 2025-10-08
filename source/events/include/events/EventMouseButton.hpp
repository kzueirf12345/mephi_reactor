#ifndef MEPHI_REACTOR_SOURCE_EVENTS_INCLUDE_EVENTS_EVENT_MOUSE_BUTTON_HPP
#define MEPHI_REACTOR_SOURCE_EVENTS_INCLUDE_EVENTS_EVENT_MOUSE_BUTTON_HPP

#include "vector/Vector.hpp"
#include "events/EventCoord.hpp"
#include <SFML/Window/Mouse.hpp>

namespace Mephi 
{

class EventMouseButton : public Mephi::EventCoord {
    public:
        static constexpr sf::Mouse::Button MOVE_BUTTON_ = sf::Mouse::Middle;
        sf::Mouse::Button button;

        EventMouseButton(Mephi::Vector2d coord, sf::Mouse::Button button)
            : Mephi::EventCoord{coord}, button{button}
        {}
};


}

#endif /* MEPHI_REACTOR_SOURCE_EVENTS_INCLUDE_EVENTS_EVENT_MOUSE_BUTTON_HPP*/