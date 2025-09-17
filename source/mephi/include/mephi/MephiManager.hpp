#ifndef MEPHI_REACTOR_SOURCE_MEPHI_INCLUDE_MEPHI_MEPHI_HPP
#define MEPHI_REACTOR_SOURCE_MEPHI_INCLUDE_MEPHI_MEPHI_HPP

#include <cstdlib>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <memory>

#include "common/ErrorHandle.hpp"
#include "molecule/Molecule.hpp"
#include "reactor/Reactor.hpp"

namespace Mephi
{

class MephiManager{
    private:
        Mephi::Reactor reactor_;
        std::vector<std::unique_ptr<Mephi::Molecule>> molecules_;
    public:
        MephiManager(const Mephi::Reactor& reactor, 
                     std::vector<std::unique_ptr<Mephi::Molecule>> molecules) 
            : reactor_{reactor}, molecules_(std::move(molecules))
        {}

        Common::Error Draw(sf::RenderWindow& window) const;
        Common::Error Update();

        [[nodiscard]]       Mephi::Reactor& GetReactor()                                        noexcept {return reactor_; }
        [[nodiscard]] const Mephi::Reactor& GetReactor()                                  const noexcept {return reactor_; }
        [[nodiscard]]       std::vector<std::unique_ptr<Mephi::Molecule>>& GetMolecules()       noexcept {return molecules_; }
        [[nodiscard]] const std::vector<std::unique_ptr<Mephi::Molecule>>& GetMolecules() const noexcept {return molecules_; }
} ;

}
#endif /*MEPHI_REACTOR_SOURCE_MEPHI_INCLUDE_MEPHI_MEPHI_HPP*/
