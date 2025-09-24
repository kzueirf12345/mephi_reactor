#ifndef MEPHI_REACTOR_SOURCE_MEPHI_INCLUDE_MEPHI_MEPHI_HPP
#define MEPHI_REACTOR_SOURCE_MEPHI_INCLUDE_MEPHI_MEPHI_HPP

#include <cstdlib>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "common/ErrorHandle.hpp"
#include "molecule/Molecule.hpp"
#include "windows/Reactor.hpp"
#include "molecule/MoleculeManager.hpp"

namespace Mephi
{

class MephiManager{
    private:
        Mephi::Reactor reactor_;
        Mephi::MoleculeManager moleculeManager_;
        Mephi::Window toolbox_;

        Common::Error HandleMoleculesWallsInteraction(Mephi::Molecule& molecule);

    public:
        explicit MephiManager(Mephi::Reactor reactor, 
                              Mephi::Window toolbox,
                              Mephi::MoleculeManager moleculeManager = Mephi::MoleculeManager(),
                              const size_t moleculeCnt = 1000) 
            : reactor_{std::move(reactor)}, moleculeManager_{std::move(moleculeManager)}, 
              toolbox_{std::move(toolbox)}
        {
            moleculeManager_.GenerateMolecules(moleculeCnt, reactor_.GetRect());
        }

        Common::Error Draw(sf::RenderWindow& window);
        Common::Error Update(const Mephi::Vector2i &mousePos);

        [[nodiscard]] const Mephi::Reactor&         GetReactor()         const noexcept {return reactor_; }
        [[nodiscard]]       Mephi::Reactor&         GetReactor()               noexcept {return reactor_; }
        [[nodiscard]] const Mephi::MoleculeManager& GetMoleculeManager() const noexcept {return moleculeManager_; }
        [[nodiscard]]       Mephi::Window&          GetToolbox()               noexcept {return toolbox_; }
        [[nodiscard]]const  Mephi::Window&          GetToolbox()         const noexcept {return toolbox_; }
} ;

}
#endif /*MEPHI_REACTOR_SOURCE_MEPHI_INCLUDE_MEPHI_MEPHI_HPP*/
