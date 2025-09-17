#ifndef MEPHI_REACTOR_SOURCE_MOLEKULE_INCLUDE_MOLECULE_MOLECULE_HPP
#define MEPHI_REACTOR_SOURCE_MOLEKULE_INCLUDE_MOLECULE_MOLECULE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdlib>

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "common/ErrorHandle.hpp"
#include "vector/Vector.hpp"

namespace Mephi
{

class Molecule {
    protected:
        Mephi::Vector2d coord_;
        Mephi::Vector2d speed_;
        uint64_t         mass_;
        sf::Color       color_;
    public:
        Molecule(const Mephi::Vector2d& startCoord, const Mephi::Vector2d& startSpeed, 
                 uint64_t startMass, const sf::Color& color)
            : coord_{startCoord}, speed_{startSpeed}, mass_{startMass}, color_{color}
        {}

        virtual Common::Error Draw(sf::RenderWindow& window) const = 0;
        virtual Common::Error Update() = 0;
};

}
#endif /*MEPHI_REACTOR_SOURCE_MOLEKULE_INCLUDE_MOLECULE_MOLECULE_HPP*/
