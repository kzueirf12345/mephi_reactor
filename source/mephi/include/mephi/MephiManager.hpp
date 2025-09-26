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
#include "windows/Plot.hpp"
#include "windows/buttons/AdjustButton.hpp"

namespace Mephi
{

class MephiManager{
    private:
        constexpr static double PLOT_DELAY_ITER_ = 40;
        Mephi::Reactor reactor_;
        Mephi::MoleculeManager moleculeManager_;
        Mephi::Window toolbox_;
        Mephi::Plot circlePlot_;
        Mephi::Plot squarePlot_;
        Mephi::Plot tempPlot_;
        double curPlotX_;

        Common::Error HandleMoleculesWallsInteraction(Mephi::Molecule& molecule);

    public:
        explicit MephiManager(Mephi::Reactor reactor, 
                              Mephi::Window toolbox,
                              Mephi::Plot circlePlot,
                              Mephi::Plot squarePlot,
                              Mephi::Plot tempPlot,
                              Mephi::MoleculeManager moleculeManager = Mephi::MoleculeManager(),
                              const size_t moleculeCnt = 1000) 
            : reactor_{std::move(reactor)}, 
              moleculeManager_{std::move(moleculeManager)}, 
              toolbox_{std::move(toolbox)}, 
              circlePlot_{std::move(circlePlot)}, 
              squarePlot_{std::move(squarePlot)}, 
              tempPlot_{std::move(tempPlot)},
              curPlotX_{-PLOT_DELAY_ITER_}
        {
            moleculeManager_.GenerateMolecules(moleculeCnt, reactor_.GetRect());
        }

        MephiManager(MephiManager&& other) noexcept 
            : reactor_(std::move(other.reactor_)), 
              moleculeManager_(std::move(other.moleculeManager_)),
              toolbox_(std::move(other.toolbox_)), 
              circlePlot_(std::move(other.circlePlot_)), 
              squarePlot_(std::move(other.squarePlot_)), 
              tempPlot_(std::move(other.tempPlot_)), 
              curPlotX_(other.curPlotX_)
        {
            if (!toolbox_.GetChildren().empty()) {
                dynamic_cast<Mephi::AdjustButton<double>*>(toolbox_.GetChildren()[0].get())->SwitchObj(reactor_.GetTemp().top);
                dynamic_cast<Mephi::AdjustButton<double>*>(toolbox_.GetChildren()[1].get())->SwitchObj(reactor_.GetTemp().left);
                dynamic_cast<Mephi::AdjustButton<double>*>(toolbox_.GetChildren()[2].get())->SwitchObj(reactor_.GetTemp().right);
                dynamic_cast<Mephi::AdjustButton<double>*>(toolbox_.GetChildren()[3].get())->SwitchObj(reactor_.GetTemp().bottom);
            }
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
