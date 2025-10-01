#ifndef MEPHI_REACTOR_SOURCE_EVENTS_INCLUDE_EVENTS_EVENT_HPP
#define MEPHI_REACTOR_SOURCE_EVENTS_INCLUDE_EVENTS_EVENT_HPP

#include "vector/Vector.hpp"

namespace Mephi 
{

class EventCoord {
    public:
        Mephi::Vector2d coord;

        explicit EventCoord(Mephi::Vector2d coord)
            : coord{std::move(coord)}
        {}
};


}

#endif /* MEPHI_REACTOR_SOURCE_EVENTS_INCLUDE_EVENTS_EVENT_HPP*/