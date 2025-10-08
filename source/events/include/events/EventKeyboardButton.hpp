#ifndef MEPHI_REACTOR_SOURCE_EVENTS_INCLUDE_EVENTS_EVENT_KEYBOARD_BUTTON_HPP
#define MEPHI_REACTOR_SOURCE_EVENTS_INCLUDE_EVENTS_EVENT_KEYBOARD_BUTTON_HPP

#include "vector/Vector.hpp"
#include "events/EventCoord.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace Mephi 
{

class EventKeyboardButton : public Mephi::EventCoord {
    public:
        sf::Keyboard::Key button;

        EventKeyboardButton(Mephi::Vector2d coord, sf::Keyboard::Key button)
            : Mephi::EventCoord{coord}, button{button}
        {}
};


}

#endif /* MEPHI_REACTOR_SOURCE_EVENTS_INCLUDE_EVENTS_EVENT_KEYBOARD_BUTTON_HPP*/