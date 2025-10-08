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
            CIRCLE  = 0     ,
            SQUARE  = 1     ,
            SIZE    = 2
        };
        MoleculeType HashCode2MoleculeType(const size_t hash_code);

    private:

        using FHandleInteraction = std::function<Common::Error(Mephi::MoleculeManager*, size_t, size_t, std::vector<bool>& toRemove)>;

        static Common::Error HandleInteractionCCWrap_(MoleculeManager* self, size_t moleculeInd1, size_t moleculeInd2, std::vector<bool>& toRemove);
        static Common::Error HandleInteractionSSWrap_(MoleculeManager* self, size_t moleculeInd1, size_t moleculeInd2, std::vector<bool>& toRemove);
        static Common::Error HandleInteractionCSWrap_(MoleculeManager* self, size_t moleculeInd1, size_t moleculeInd2, std::vector<bool>& toRemove);

        Common::Error HandleInteractionCC_(size_t moleculeInd1, size_t moleculeInd2, std::vector<bool>& toRemove);
        Common::Error HandleInteractionSS_(size_t moleculeInd1, size_t moleculeInd2, std::vector<bool>& toRemove);
        Common::Error HandleInteractionCS_(size_t moleculeInd1, size_t moleculeInd2, std::vector<bool>& toRemove);

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

        Common::Error GenerateMolecules(const size_t count, const double maxSpeed, 
                                        const Mephi::Rect& rect);
        Common::Error DeleteMolecules  (const size_t count);

        [[nodiscard]]       size_t                                         GetMoleculesCnt() const noexcept {return molecules_.size();}
        [[nodiscard]]       size_t                                         GetCircleCnt()    const noexcept {return circleCnt_;}
        [[nodiscard]]       size_t                                         GetSquareCnt()    const noexcept {return squareCnt_;}
        [[nodiscard]] const std::vector<std::unique_ptr<Mephi::Molecule>>& GetMolecules()    const noexcept {return molecules_;}
        [[nodiscard]]       double                                         TotalEnergy()     const noexcept;
        [[nodiscard]]       double                                         TotalMass()       const noexcept;
} ;

}
#endif /*MEPHI_REACTOR_SOURCE_MOLECULE_INCLUDE_MOLECULE_MOLECULE_MANAGER_HPP*/
