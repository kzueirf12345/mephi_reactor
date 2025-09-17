
#include "common/ErrorHandle.hpp"
#include "vector/Vector.hpp"
#include "mephi/MephiManager.hpp"

Common::Error Mephi::MephiManager::Draw(sf::RenderWindow& window) const {

    ERROR_HANDLE(reactor_.Draw(window));

    for (const auto& molecule : molecules_) {
        ERROR_HANDLE(molecule->Draw(window));
    }

    return Common::Error::SUCCESS;
}

Common::Error Mephi::MephiManager::Update() {
    for (const auto& molecule : molecules_) {
        ERROR_HANDLE(molecule->Update());

        const Mephi::Vector2d curCoord = molecule->GetCoord();
        const Mephi::Vector2d curSpeed = molecule->GetSpeed();
        if (molecule->LeftX() <= reactor_.GetLeftCorner().x) {
            molecule->GetCoord().x = reactor_.GetLeftCorner().x  + (reactor_.GetLeftCorner().x  - molecule->LeftX());
            molecule->GetSpeed().x = -curSpeed.x;
        }
        if (reactor_.GetRightCorner().x <= molecule->RightX()) {
            molecule->GetCoord().x = reactor_.GetRightCorner().x - (molecule->RightX() - reactor_.GetRightCorner().x);
            molecule->GetSpeed().x = -curSpeed.x;
        }
        if (molecule->TopY() <= reactor_.GetLeftCorner().y) {
            molecule->GetCoord().y = reactor_.GetLeftCorner().y  + (reactor_.GetLeftCorner().y  - molecule->TopY());
            molecule->GetSpeed().y = -curSpeed.y;
        }
        if (reactor_.GetRightCorner().x <= molecule->BottomY()) {
            molecule->GetCoord().y = reactor_.GetRightCorner().y - (molecule->BottomY() - reactor_.GetRightCorner().y);
            molecule->GetSpeed().y = -curSpeed.y;
        }
    }
    return Common::Error::SUCCESS;
}