#ifndef MEPHI_REACTOR_SOURCE_MEPHI_INCLUDE_MEPHI_MEPHI_HPP
#define MEPHI_REACTOR_SOURCE_MEPHI_INCLUDE_MEPHI_MEPHI_HPP

#include <cstdlib>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "common/ErrorHandle.hpp"
#include "window/Reactor.hpp"
#include "molecule/MoleculeManager.hpp"

namespace Mephi
{

class MephiManager{
    private:
        Mephi::Reactor reactor_;
        Mephi::MoleculeManager moleculeManager_;

    public:
        explicit MephiManager(Mephi::Reactor reactor, 
                              Mephi::MoleculeManager moleculeManager = Mephi::MoleculeManager(),
                              const size_t moleculeCnt = 1000) 
            : reactor_{std::move(reactor)}, moleculeManager_{std::move(moleculeManager)}
        {
            moleculeManager_.GenerateMolecules(moleculeCnt, reactor_.GetRect());
        }

        Common::Error Draw(sf::RenderWindow& window) const;
        Common::Error Update(const Mephi::Vector2i &mousePos);

        [[nodiscard]] const Mephi::Reactor&         GetReactor()         const noexcept {return reactor_; }
        [[nodiscard]] const Mephi::MoleculeManager& GetMoleculeManager() const noexcept {return moleculeManager_; }
} ;

}
#endif /*MEPHI_REACTOR_SOURCE_MEPHI_INCLUDE_MEPHI_MEPHI_HPP*/
