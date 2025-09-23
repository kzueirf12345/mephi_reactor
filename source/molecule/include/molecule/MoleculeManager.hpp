#ifndef MEPHI_REACTOR_SOURCE_MOLECULE_INCLUDE_MOLECULE_MOLECULE_MANAGER_HPP
#define MEPHI_REACTOR_SOURCE_MOLECULE_INCLUDE_MOLECULE_MOLECULE_MANAGER_HPP

#include <cstdlib>
#include <memory>

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

class MoleculeManager{
    private:
        std::vector<std::unique_ptr<Mephi::Molecule>> molecules_;
        size_t circleCnt_;
        size_t squareCnt_;

    public:
        enum MoleculeType {
            UNKNOWN = -1    ,
            CIRCLE  = 0     ,
            SQUARE          ,
            SIZE
        };
        MoleculeType HashCode2MoleculeType(const size_t hash_code);

    private:

        using FHandleInteraction = Common::Error(MoleculeManager::*)(size_t, size_t);

        Common::Error HandleInteractionCC_(size_t moleculeInd1, size_t moleculeInd2);
        Common::Error HandleInteractionSS_(size_t moleculeInd1, size_t moleculeInd2);
        Common::Error HandleInteractionCS_(size_t moleculeInd1, size_t moleculeInd2);

        const static FHandleInteraction 
            handleIntercationFuncsTable_
                [Mephi::MoleculeManager::MoleculeType::SIZE]
                [Mephi::MoleculeManager::MoleculeType::SIZE];
    
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
        [[nodiscard]]       double                                         TotalEnergy() const noexcept;
} ;

}
#endif /*MEPHI_REACTOR_SOURCE_MOLECULE_INCLUDE_MOLECULE_MOLECULE_MANAGER_HPP*/
