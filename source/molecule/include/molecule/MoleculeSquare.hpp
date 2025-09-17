#ifndef MEPHI_REACTOR_SOURCE_MOLEKULE_INCLUDE_MOLECULE_MOLECULE_SQUARE_HPP
#define MEPHI_REACTOR_SOURCE_MOLEKULE_INCLUDE_MOLECULE_MOLECULE_SQUARE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdlib>

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <numbers>

#include "common/ErrorHandle.hpp"
#include "vector/Vector.hpp"
#include "Molecule.hpp"

namespace Mephi
{

class MoleculeSquare: public Mephi::Molecule {
    private:
        uint64_t side_;
        uint64_t radius_;
    public:
        MoleculeSquare(const Mephi::Vector2d& startCoord, const Mephi::Vector2d& startSpeed, 
                       uint64_t startMass, const sf::Color& color, const uint64_t radius)
            : Mephi::Molecule{startCoord, startSpeed, startMass, color}, radius_{radius}, 
              side_{uint64_t(radius * std::numbers::sqrt2)}
        {}

        virtual Common::Error Draw(sf::RenderWindow& window) const override final;
        virtual Common::Error Update() override final;

        [[nodiscard]]         const uint64_t& GetRadius() const noexcept                {return radius_;}
        [[nodiscard]]               uint64_t& GetRadius()       noexcept                {return radius_;}
        [[nodiscard]] virtual       uint64_t  LeftX()     const noexcept override final {return coord_.x - radius_ * std::numbers::sqrt2;}
        [[nodiscard]] virtual       uint64_t  RightX()    const noexcept override final {return coord_.x + radius_ * std::numbers::sqrt2;}
        [[nodiscard]] virtual       uint64_t  TopY()      const noexcept override final {return coord_.y - radius_ * std::numbers::sqrt2;}
        [[nodiscard]] virtual       uint64_t  BottomY()   const noexcept override final {return coord_.y + radius_ * std::numbers::sqrt2;}
};

}
#endif /*MEPHI_REACTOR_SOURCE_MOLEKULE_INCLUDE_MOLECULE_MOLECULE_SQUARE_HPP*/
