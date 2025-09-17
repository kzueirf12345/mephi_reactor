#ifndef MEPHI_REACTOR_SOURCE_REACTOR_INCLUDE_REACTOR_REACTOR_HPP
#define MEPHI_REACTOR_SOURCE_REACTOR_INCLUDE_REACTOR_REACTOR_HPP

#include <cstdlib>

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>


#include "figures/Rect.hpp"

namespace Mephi
{

class Reactor: public Mephi::Rect {
    private:
        ssize_t resistance_; // TODO
    public:
        explicit Reactor(const Mephi::Rect& rect) 
            : Mephi::Rect{rect}
        {}
};

}
#endif /*MEPHI_REACTOR_SOURCE_REACTOR_INCLUDE_REACTOR_REACTOR_HPP*/
