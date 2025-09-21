#ifndef MEPHI_REACTOR_SOURCE_MOLECULE_INCLUDE_MOLECULE_MOLECULE_MANAGER_HPP
#define MEPHI_REACTOR_SOURCE_MOLECULE_INCLUDE_MOLECULE_MOLECULE_MANAGER_HPP

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
#include "figures/Rect.hpp"

namespace Mephi
{

class MoleculeManager;

using FHandleInteraction = std::function<Common::Error(Mephi::MoleculeManager& manager, size_t& moleculeInd1, size_t& moleculeInd2)>;
using THandleIntercationFuncsTable = std::unordered_map<size_t, std::unordered_map<size_t, FHandleInteraction>>;

class MoleculeManager{
    private:
        std::vector<std::unique_ptr<Mephi::Molecule>> molecules_;
        size_t circleCnt_;
        size_t squareCnt_;
    
        static THandleIntercationFuncsTable handleIntercationFuncsTable_;
        Common::Error HandleInteractionCC_(size_t& moleculeInd1, size_t& moleculeInd2);
        Common::Error HandleInteractionSS_(size_t& moleculeInd1, size_t& moleculeInd2);
        Common::Error HandleInteractionCS_(size_t& moleculeInd1, size_t& moleculeInd2);

    public:
        explicit MoleculeManager(std::vector<std::unique_ptr<Mephi::Molecule>> molecules = {}) 
            : molecules_(std::move(molecules)), circleCnt_(0), squareCnt_(0)
        {}

        Common::Error Draw(sf::RenderWindow& window) const;
        Common::Error HandleInteraction_();

        Common::Error GenerateMolecules(const size_t count, const Mephi::Rect& rect);

        [[nodiscard]]       size_t                                         GetCircleCnt() const noexcept {return circleCnt_;}
        [[nodiscard]]       size_t                                         GetSquareCnt() const noexcept {return squareCnt_;}
        [[nodiscard]] const std::vector<std::unique_ptr<Mephi::Molecule>>& GetMolecules() const noexcept {return molecules_;}
} ;

}
#endif /*MEPHI_REACTOR_SOURCE_MOLECULE_INCLUDE_MOLECULE_MOLECULE_MANAGER_HPP*/
