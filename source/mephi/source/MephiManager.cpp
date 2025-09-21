
#include <memory>

#include "common/ErrorHandle.hpp"
#include "molecule/Molecule.hpp"
#include "vector/Vector.hpp"
#include "mephi/MephiManager.hpp"

Common::Error Mephi::MephiManager::Draw(sf::RenderWindow& window) const {

    ERROR_HANDLE(reactor_.Draw(window));
    ERROR_HANDLE(moleculeManager_.Draw(window));

    return Common::Error::SUCCESS;
}

Common::Error Mephi::MephiManager::Update() {
    for (const auto& molecule : moleculeManager_.GetMolecules()) {
        ERROR_HANDLE(molecule->Update());

        double tempDiff = 0;
        double energyDiff = 0;

        const Mephi::Vector2d curCoord = molecule->GetCoord();
        const Mephi::Vector2d curSpeed = molecule->GetSpeed();
        if (molecule->LeftX() <= reactor_.GetLeftCorner().x && molecule->GetSpeed().x < 0) {
            tempDiff = reactor_.GetTemp().left - molecule->KinEnergyX();
            energyDiff = reactor_.GetAccom() * tempDiff;
            reactor_.GetTemp().left -= energyDiff;

            molecule->GetCoord().x = reactor_.GetLeftCorner().x + molecule->GetRadius()  + (reactor_.GetLeftCorner().x  - molecule->LeftX());
            molecule->GetSpeed().x = -curSpeed.x;
        }
        if (reactor_.GetRightCorner().x <= molecule->RightX() && molecule->GetSpeed().x > 0) {
            tempDiff = reactor_.GetTemp().right - molecule->KinEnergyX();
            energyDiff = reactor_.GetAccom() * tempDiff;
            reactor_.GetTemp().right -= energyDiff;

            molecule->GetCoord().x = reactor_.GetRightCorner().x - molecule->GetRadius() - (molecule->RightX() - reactor_.GetRightCorner().x);
            molecule->GetSpeed().x = -curSpeed.x;
        }
        if (molecule->TopY() <= reactor_.GetLeftCorner().y && molecule->GetSpeed().y < 0) {
            tempDiff = reactor_.GetTemp().top - molecule->KinEnergyX();
            energyDiff = reactor_.GetAccom() * tempDiff;
            reactor_.GetTemp().top -= energyDiff;

            molecule->GetCoord().y = reactor_.GetLeftCorner().y + molecule->GetRadius() + (reactor_.GetLeftCorner().y  - molecule->TopY());
            molecule->GetSpeed().y = -curSpeed.y;
        }
        if (reactor_.GetRightCorner().y <= molecule->BottomY() && molecule->GetSpeed().y > 0) {
            tempDiff = reactor_.GetTemp().bottom - molecule->KinEnergyX();
            energyDiff = reactor_.GetAccom() * tempDiff;
            reactor_.GetTemp().bottom -= energyDiff;

            molecule->GetCoord().y = reactor_.GetRightCorner().y - molecule->GetRadius() - (molecule->BottomY() - reactor_.GetRightCorner().y);
            molecule->GetSpeed().y = -curSpeed.y;
        }

        molecule->GetSpeed() *= 1 + energyDiff / molecule->KinEnergy();

    }

    ERROR_HANDLE(moleculeManager_.HandleInteraction_());

    return Common::Error::SUCCESS;
}