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
        double           mass_;
        double         radius_;
        sf::Color       color_;
    public:
        static constexpr double START_RADIUS = 1;
        Molecule(const Mephi::Vector2d& startCoord, const Mephi::Vector2d& startSpeed, 
                 double startMass, const sf::Color& color)
            : coord_{startCoord}, speed_{startSpeed}, mass_{startMass}, color_{color}, 
              radius_(startMass)
        {}

        [[nodiscard]] virtual size_t GetTypeId() const noexcept;

        virtual Common::Error Draw(sf::RenderWindow& window) const = 0;
        virtual Common::Error Update();

        [[nodiscard]]       double           KinEnergy() const noexcept {return mass_ * speed_.Len2() / 2;}
        [[nodiscard]]       double           KinEnergyX()const noexcept {return mass_ * speed_.x * speed_.x / 2;}
        [[nodiscard]]       double           KinEnergyY()const noexcept {return mass_ * speed_.y * speed_.y / 2;}

        [[nodiscard]] const Mephi::Vector2d& GetCoord() const noexcept {return coord_;}
        [[nodiscard]]       Mephi::Vector2d& GetCoord()       noexcept {return coord_;}
        [[nodiscard]] const Mephi::Vector2d& GetSpeed() const noexcept {return speed_;}
        [[nodiscard]]       Mephi::Vector2d& GetSpeed()       noexcept {return speed_;}
        [[nodiscard]] const double&          GetMass()  const noexcept {return mass_;}
        [[nodiscard]] const double&          GetRadius()const noexcept {return radius_;}
};

bool IsIntersect(const Mephi::Molecule& molecule1, 
                 const Mephi::Molecule& molecule2) noexcept;

}
#endif /*MEPHI_REACTOR_SOURCE_MOLEKULE_INCLUDE_MOLECULE_MOLECULE_HPP*/
