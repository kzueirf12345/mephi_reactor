#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_REACTOR_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_REACTOR_HPP

#include <cstdlib>

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "windows/Window.hpp"
#include "molecule/MoleculeManager.hpp"

namespace Mephi
{

struct Sides {
    double left;
    double top;
    double right;
    double bottom;

    [[nodiscard]] double Average() const noexcept {
        return (left + top + right + bottom) / 4;
    }
};

class Reactor: public Mephi::Window {
    private:
    Mephi::MoleculeManager moleculeManager_;
    
    Sides wallEnergy_;
    double accom_;
    
    Common::Error HandleWallCollisions(Mephi::Molecule& molecule, const size_t maxChangeCycle = 2);
    public:
        explicit Reactor(Mephi::Rect rect, 
                         Mephi::MoleculeManager moleculeManager = std::move(Mephi::MoleculeManager()), 
                         const double accom = 0.1) 
            : Mephi::Window{std::move(rect)}, moleculeManager_{std::move(moleculeManager)}, wallEnergy_{0}, 
              accom_{accom}
        {}

        [[nodiscard]] const Mephi::Sides& GetTemp()  const noexcept {return wallEnergy_;}
        [[nodiscard]]       double        GetAccom() const noexcept {return accom_;}
        [[nodiscard]] const Mephi::MoleculeManager& GetMoleculeManager() const noexcept {return moleculeManager_;}

        Common::Error GenerateMolecules(const size_t count, const double maxSpeed);
        Common::Error Update();
        virtual Common::Error Draw(sf::RenderWindow& window) const override final;

};

}
#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_REACTOR_HPP*/
