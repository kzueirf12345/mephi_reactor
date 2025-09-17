#ifndef MEPHI_REACTOR_SOURCE_MEPHI_INCLUDE_MEPHI_MEPHI_HPP
#define MEPHI_REACTOR_SOURCE_MEPHI_INCLUDE_MEPHI_MEPHI_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdlib>

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "common/ErrorHandle.hpp"
#include "reactor/Reactor.hpp"

namespace Mephi
{

class MephiManager{
    private:
        Mephi::Reactor reactor_;
    public:
        explicit MephiManager(const Mephi::Reactor& reactor) 
            : reactor_{reactor}
        {}

        Common::Error Draw(sf::RenderWindow& window) const;

        [[nodiscard]]       Mephi::Reactor& GetReactor()       noexcept {return reactor_; }
        [[nodiscard]] const Mephi::Reactor& GetReactor() const noexcept {return reactor_; }
} ;

}
#endif /*MEPHI_REACTOR_SOURCE_MEPHI_INCLUDE_MEPHI_MEPHI_HPP*/
