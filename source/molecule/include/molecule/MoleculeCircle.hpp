#ifndef MEPHI_REACTOR_SOURCE_MOLEKULE_INCLUDE_MOLECULE_MOLECULE_CIRCLE_HPP
#define MEPHI_REACTOR_SOURCE_MOLEKULE_INCLUDE_MOLECULE_MOLECULE_CIRCLE_HPP

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

class MoleculeCircle: public Mephi::Molecule {
    private:
    public:
        static constexpr uint64_t START_RADIUS = 1;
        MoleculeCircle(const Mephi::Vector2d& startCoord, const Mephi::Vector2d& startSpeed, 
                       const sf::Color& color)
            : Mephi::Molecule{startCoord, startSpeed, 1, color}
        {
            radius_ += START_RADIUS;
        }

        virtual Common::Error Draw(sf::RenderWindow& window) const override final;

        [[nodiscard]]         const uint64_t& GetRadius() const noexcept                {return radius_;}
        [[nodiscard]]               uint64_t& GetRadius()       noexcept                {return radius_;}
        [[nodiscard]] virtual       uint64_t  LeftX()     const noexcept override final {return coord_.x - radius_;}
        [[nodiscard]] virtual       uint64_t  RightX()    const noexcept override final {return coord_.x + radius_;}
        [[nodiscard]] virtual       uint64_t  TopY()      const noexcept override final {return coord_.y - radius_;}
        [[nodiscard]] virtual       uint64_t  BottomY()   const noexcept override final {return coord_.y + radius_;}
};

}
#endif /*MEPHI_REACTOR_SOURCE_MOLEKULE_INCLUDE_MOLECULE_MOLECULE_CIRCLE_HPP*/
