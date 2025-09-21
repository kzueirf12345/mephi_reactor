
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

        const Mephi::Vector2d curCoord = molecule->GetCoord();
        const Mephi::Vector2d curSpeed = molecule->GetSpeed();
        if (molecule->LeftX() <= reactor_.GetLeftCorner().x && molecule->GetSpeed().x < 0) {
            molecule->GetCoord().x = reactor_.GetLeftCorner().x + molecule->GetRadius()  + (reactor_.GetLeftCorner().x  - molecule->LeftX());
            molecule->GetSpeed().x = -curSpeed.x;
        }
        if (reactor_.GetRightCorner().x <= molecule->RightX() && molecule->GetSpeed().x > 0) {
            molecule->GetCoord().x = reactor_.GetRightCorner().x - molecule->GetRadius() - (molecule->RightX() - reactor_.GetRightCorner().x);
            molecule->GetSpeed().x = -curSpeed.x;
        }
        if (molecule->TopY() <= reactor_.GetLeftCorner().y && molecule->GetSpeed().y < 0) {
            molecule->GetCoord().y = reactor_.GetLeftCorner().y + molecule->GetRadius() + (reactor_.GetLeftCorner().y  - molecule->TopY());
            molecule->GetSpeed().y = -curSpeed.y;
        }
        if (reactor_.GetRightCorner().y <= molecule->BottomY() && molecule->GetSpeed().y > 0) {
            molecule->GetCoord().y = reactor_.GetRightCorner().y - molecule->GetRadius() - (molecule->BottomY() - reactor_.GetRightCorner().y);
            molecule->GetSpeed().y = -curSpeed.y;
        }
    }

    ERROR_HANDLE(moleculeManager_.HandleInteraction_());

    return Common::Error::SUCCESS;
}