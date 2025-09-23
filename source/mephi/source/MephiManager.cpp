
#include <memory>

#include "common/ErrorHandle.hpp"
#include "molecule/Molecule.hpp"
#include "vector/Vector.hpp"
#include "mephi/MephiManager.hpp"
#include "windows/Reactor.hpp"
// #include "window/Window.hpp"

Common::Error Mephi::MephiManager::Draw(sf::RenderWindow& window) {

    ERROR_HANDLE(reactor_.Draw(window));
    ERROR_HANDLE(moleculeManager_.Draw(window));

    return Common::Error::SUCCESS;
}

Common::Error Mephi::MephiManager::HandleMoleculesWallsInteraction(Mephi::Molecule& molecule) {
    double tempDiff = 0;
    double energyDiff = 0;

    const Mephi::Vector2d curCoord = molecule.GetCoord();
    const Mephi::Vector2d curSpeed = molecule.GetSpeed();

    if (molecule.LeftX() <= reactor_.GetRect().GetLeftCorner().x && molecule.GetSpeed().x < 0) {
        tempDiff   = reactor_.GetTemp().left - molecule.KinEnergyX();
        energyDiff = reactor_.GetAccom() * tempDiff;
        reactor_.GetTemp().left -= energyDiff;

        molecule.GetCoord().x =  reactor_.GetRect().GetLeftCorner().x + molecule.GetRadius()
                              + (reactor_.GetRect().GetLeftCorner().x  - molecule.LeftX());
        molecule.GetSpeed().x = -curSpeed.x;
    }

    if (reactor_.GetRect().GetRightCorner().x <= molecule.RightX() && molecule.GetSpeed().x > 0) {
        tempDiff = reactor_.GetTemp().right - molecule.KinEnergyX();
        energyDiff = reactor_.GetAccom() * tempDiff;
        reactor_.GetTemp().right -= energyDiff;

        molecule.GetCoord().x =  reactor_.GetRect().GetRightCorner().x - molecule.GetRadius() 
                              - (molecule.RightX() - reactor_.GetRect().GetRightCorner().x);
        molecule.GetSpeed().x = -curSpeed.x;
    }

    if (molecule.TopY() <= reactor_.GetRect().GetLeftCorner().y && molecule.GetSpeed().y < 0) {
        tempDiff = reactor_.GetTemp().top - molecule.KinEnergyX();
        energyDiff = reactor_.GetAccom() * tempDiff;
        reactor_.GetTemp().top -= energyDiff;

        molecule.GetCoord().y =  reactor_.GetRect().GetLeftCorner().y + molecule.GetRadius() 
                              + (reactor_.GetRect().GetLeftCorner().y  - molecule.TopY());
        molecule.GetSpeed().y = -curSpeed.y;
    }

    if (reactor_.GetRect().GetRightCorner().y <= molecule.BottomY() && molecule.GetSpeed().y > 0) {
        tempDiff = reactor_.GetTemp().bottom - molecule.KinEnergyX();
        energyDiff = reactor_.GetAccom() * tempDiff;
        reactor_.GetTemp().bottom -= energyDiff;

        molecule.GetCoord().y =  reactor_.GetRect().GetRightCorner().y - molecule.GetRadius() 
                              - (molecule.BottomY() - reactor_.GetRect().GetRightCorner().y);
        molecule.GetSpeed().y = -curSpeed.y;
    }

    molecule.GetSpeed() *= 1 + energyDiff / molecule.KinEnergy();

    return Common::Error::SUCCESS;
}

Common::Error Mephi::MephiManager::Update(const Mephi::Vector2i &mousePos) {

    Mephi::Vector2i shift = reactor_.HandleDrag(mousePos);

    for (const auto& molecule : moleculeManager_.GetMolecules()) {
        molecule->GetCoord() += static_cast<Mephi::Vector2d>(shift);
        
        ERROR_HANDLE(molecule->Update());

        ERROR_HANDLE(HandleMoleculesWallsInteraction(*molecule));
    }

    ERROR_HANDLE(moleculeManager_.HandleInteraction_());

    return Common::Error::SUCCESS;
}