#ifndef MEPHI_REACTOR_SOURCE_MEPHI_INCLUDE_MEPHI_MEPHI_HPP
#define MEPHI_REACTOR_SOURCE_MEPHI_INCLUDE_MEPHI_MEPHI_HPP

#include <cstdlib>
#include <memory>
#include <functional>
#include <unordered_map>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "common/ErrorHandle.hpp"
#include "molecule/Molecule.hpp"
#include "reactor/Reactor.hpp"

namespace Mephi
{

class MephiManager;

using FHandleInteraction = std::function<Common::Error(Mephi::MephiManager& manager, size_t& moleculeInd1, size_t& moleculeInd2)>;
using THandleIntercationFuncsTable = std::unordered_map<size_t, std::unordered_map<size_t, FHandleInteraction>>; // TODO enum. Сравнить скорости 

class MephiManager{
    private:
        Mephi::Reactor reactor_;
        std::vector<std::unique_ptr<Mephi::Molecule>> molecules_;
    
        static THandleIntercationFuncsTable handleIntercationFuncsTable_;
        Common::Error HandleInteraction_();
        Common::Error HandleInteractionCC_(size_t& moleculeInd1, size_t& moleculeInd2);
        Common::Error HandleInteractionSS_(size_t& moleculeInd1, size_t& moleculeInd2);
        Common::Error HandleInteractionCS_(size_t& moleculeInd1, size_t& moleculeInd2);


    public:
        explicit MephiManager(const Mephi::Reactor& reactor, 
                              std::vector<std::unique_ptr<Mephi::Molecule>> molecules = {}) 
            : reactor_{reactor}, molecules_(std::move(molecules))
        {}

        Common::Error Draw(sf::RenderWindow& window) const;
        Common::Error Update();

        Common::Error GenerateMolecules(const size_t count);

        [[nodiscard]]       Mephi::Reactor& GetReactor()                                        noexcept {return reactor_; }
        [[nodiscard]] const Mephi::Reactor& GetReactor()                                  const noexcept {return reactor_; }
        [[nodiscard]]       std::vector<std::unique_ptr<Mephi::Molecule>>& GetMolecules()       noexcept {return molecules_; }
        [[nodiscard]] const std::vector<std::unique_ptr<Mephi::Molecule>>& GetMolecules() const noexcept {return molecules_; }
} ;

}
#endif /*MEPHI_REACTOR_SOURCE_MEPHI_INCLUDE_MEPHI_MEPHI_HPP*/
