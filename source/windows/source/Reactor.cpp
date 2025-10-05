#include "windows/Reactor.hpp"
#include "common/ErrorHandle.hpp"
#include "vector/Vector.hpp"
#include "windows/Window.hpp"
#include "molecule/MoleculeManager.hpp"

Common::Error Mephi::Reactor::GenerateMolecules(const size_t count, const double maxSpeed) {
    Mephi::Rect genRect(rect_);
    genRect.GetLeftCorner()  = AbsoluteCoord();

    ERROR_HANDLE(moleculeManager_.GenerateMolecules(count, maxSpeed, genRect));

    return Common::Error::SUCCESS;
}

Common::Error Mephi::Reactor::DeleteMolecules(const size_t count) {

    ERROR_HANDLE(moleculeManager_.DeleteMolecules(count));

    return Common::Error::SUCCESS;
}

Common::Error Mephi::Reactor::Draw(sf::RenderWindow& window) const {

    ERROR_HANDLE(this->Mephi::Window::Draw(window));

    ERROR_HANDLE(moleculeManager_.Draw(window));

    return Common::Error::SUCCESS;
}

static double CalculateWallCollisionEnergy_(double& wallE, const double molE, const double accom) {
    const double maxdE = accom * std::abs(molE - wallE);
    const double dE = (molE > wallE ? 1 : -1) * std::min(maxdE, std::abs(molE - wallE) / 2);
    wallE += dE;

    const double newMolE = molE - dE;
    const double speedFactor = newMolE > 0 ? std::sqrt(newMolE / molE) : 1;

    return speedFactor;
}

Common::Error Mephi::Reactor::HandleWallCollisions(Mephi::Molecule& molecule, const size_t maxChangeCycle) {
    auto& coord = molecule.GetCoord();
    auto& speed = molecule.GetSpeed();
    const uint64_t radius = molecule.GetRadius();
    const uint64_t mass = molecule.GetMass();
    
    const double   leftBound = AbsoluteCoord().x ;
    const double  rightBound = AbsoluteCoord().x + rect_.Width();
    const double    topBound = AbsoluteCoord().y ;
    const double bottomBound = AbsoluteCoord().y + rect_.Height();

    bool isChanged = false;
    size_t changeCycleCnt = 0;

    do {
        ++changeCycleCnt;
        if (coord.x - radius <= leftBound && speed.x < 0) {
            const double speedFactor = CalculateWallCollisionEnergy_(
                wallEnergy_.left, 
                molecule.KinEnergy(), 
                accom_
            );
            
            coord.x = leftBound + radius + (leftBound - (coord.x - radius)) * speedFactor;
            speed *= speedFactor;
            speed.x = -speed.x;
            isChanged = true;
        }
        
        if (coord.x + radius >= rightBound && speed.x > 0) {
            const double speedFactor = CalculateWallCollisionEnergy_(
                wallEnergy_.right, 
                molecule.KinEnergy(), 
                accom_
            );
            
            coord.x = rightBound - radius + (rightBound - (coord.x + radius)) * speedFactor;
            speed *= speedFactor;
            speed.x = -speed.x;
            isChanged = true;
        }

        if (coord.y - radius <= topBound && speed.y < 0) {
            const double speedFactor = CalculateWallCollisionEnergy_(
                wallEnergy_.top, 
                molecule.KinEnergy(), 
                accom_
            );
            
            coord.y = topBound + radius + (topBound - (coord.y - radius)) * speedFactor;
            speed *= speedFactor;
            speed.y = -speed.y;
            isChanged = true;
        }
        
        if (coord.y + radius >= bottomBound && speed.y > 0) {
            const double speedFactor = CalculateWallCollisionEnergy_(
                wallEnergy_.bottom, 
                molecule.KinEnergy(), 
                accom_
            );
            
            coord.y = bottomBound - radius + (bottomBound - (coord.y + radius)) * speedFactor;
            speed *= speedFactor;
            speed.y = -speed.y;
            isChanged = true;
        }
    } while (isChanged && changeCycleCnt < maxChangeCycle);

    return Common::Error::SUCCESS;
}

Common::Error Mephi::Reactor::Update() {
    ERROR_HANDLE(Mephi::Window::Update());

    ERROR_HANDLE(moleculeManager_.HandleInteraction_());

    for (const auto& molecule : moleculeManager_.GetMolecules()) {
        ERROR_HANDLE(molecule->Update());
        
        ERROR_HANDLE(HandleWallCollisions(*molecule.get()));
    }

    return Common::Error::SUCCESS;
}


bool Mephi::Reactor::OnMouseDrag(Mephi::EventCoord event) {
    if (isDraggable_ && isSelected_) {
        const Mephi::Vector2d shift = event.coord - prevMousePos_;

        for (auto& molecule : moleculeManager_.GetMolecules()) {
            molecule->GetCoord() += shift;
        }
    }
    
    return Mephi::Window::OnMouseDrag(event);
}