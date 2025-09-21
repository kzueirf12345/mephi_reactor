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

struct Sides {
    double left;
    double top;
    double right;
    double bottom;
};

class Reactor: public Mephi::Rect {
    private:
        Sides temp_;
        double accom_;
    public:
        explicit Reactor(const Mephi::Rect& rect, const double accom = 0.5) 
            : Mephi::Rect{rect}, temp_{0}, accom_{accom}
        {}

        [[nodiscard]] const Mephi::Sides& GetTemp() const noexcept {return temp_;}
        [[nodiscard]]       Mephi::Sides& GetTemp()       noexcept {return temp_;}
        [[nodiscard]]       double        GetAccom() const noexcept {return accom_;}
};

}
#endif /*MEPHI_REACTOR_SOURCE_REACTOR_INCLUDE_REACTOR_REACTOR_HPP*/
