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
        uint64_t       radius_;
    public:
        Molecule(const Mephi::Vector2d& startCoord, const Mephi::Vector2d& startSpeed, 
                 uint64_t startMass, const sf::Color& color)
            : coord_{startCoord}, speed_{startSpeed}, mass_{startMass}, color_{color}, 
              radius_(startMass)
        {}

        [[nodiscard]] virtual size_t GetTypeId() const noexcept;

        virtual Common::Error Draw(sf::RenderWindow& window) const = 0;
        virtual Common::Error Update() = 0;

        [[nodiscard]] virtual uint64_t  LeftX()     const noexcept = 0;
        [[nodiscard]] virtual uint64_t  RightX()    const noexcept = 0;
        [[nodiscard]] virtual uint64_t  TopY()      const noexcept = 0;
        [[nodiscard]] virtual uint64_t  BottomY()   const noexcept = 0;

        [[nodiscard]] const Mephi::Vector2d& GetCoord() const noexcept {return coord_;}
        [[nodiscard]]       Mephi::Vector2d& GetCoord()       noexcept {return coord_;}
        [[nodiscard]] const Mephi::Vector2d& GetSpeed() const noexcept {return speed_;}
        [[nodiscard]]       Mephi::Vector2d& GetSpeed()       noexcept {return speed_;}
        [[nodiscard]] const uint64_t&        GetMass()  const noexcept {return mass_;}
        [[nodiscard]] const uint64_t&        GetRadius()const noexcept {return radius_;}
};

bool IsIntersect(const Mephi::Molecule& molecule1, 
                 const Mephi::Molecule& molecule2) noexcept;

}
#endif /*MEPHI_REACTOR_SOURCE_MOLEKULE_INCLUDE_MOLECULE_MOLECULE_HPP*/
