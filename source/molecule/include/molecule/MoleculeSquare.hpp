#ifndef MEPHI_REACTOR_SOURCE_MOLEKULE_INCLUDE_MOLECULE_MOLECULE_SQUARE_HPP
#define MEPHI_REACTOR_SOURCE_MOLEKULE_INCLUDE_MOLECULE_MOLECULE_SQUARE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdlib>

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "common/ErrorHandle.hpp"
#include "vector/Vector.hpp"
#include "Molecule.hpp"

namespace Mephi
{

class MoleculeSquare: public Mephi::Molecule {
    private:
    public:
        MoleculeSquare(const Mephi::Vector2d& startCoord, const Mephi::Vector2d& startSpeed, 
                       double startMass, const sf::Color& color)
            : Mephi::Molecule{startCoord, startSpeed, startMass, color}
        {
            radius_ += START_RADIUS;
        }

        virtual Common::Error Draw(sf::RenderWindow& window) const override final;

        Common::Error IncreaseMass(double addNum) noexcept;
        // [[nodiscard]] virtual       uint64_t  LeftX()     const noexcept override final {return coord_.x - radius_;}
        // [[nodiscard]] virtual       uint64_t  RightX()    const noexcept override final {return coord_.x + radius_;}
        // [[nodiscard]] virtual       uint64_t  TopY()      const noexcept override final {return coord_.y - radius_;}
        // [[nodiscard]] virtual       uint64_t  BottomY()   const noexcept override final {return coord_.y + radius_;}
};

}
#endif /*MEPHI_REACTOR_SOURCE_MOLEKULE_INCLUDE_MOLECULE_MOLECULE_SQUARE_HPP*/
