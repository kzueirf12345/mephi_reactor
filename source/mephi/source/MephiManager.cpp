
#include <memory>
#include <random>

#include "common/ErrorHandle.hpp"
#include "molecule/Molecule.hpp"
#include "molecule/MoleculeCircle.hpp"
#include "molecule/MoleculeSquare.hpp"
#include "vector/Vector.hpp"
#include "mephi/MephiManager.hpp"

Mephi::THandleIntercationFuncsTable Mephi::MephiManager::handleIntercationFuncsTable_ = []() {
    const auto circleId = typeid(Mephi::MoleculeCircle).hash_code();
    const auto squareId = typeid(Mephi::MoleculeSquare).hash_code();
    
    std::unordered_map<size_t, std::unordered_map<size_t, Mephi::FHandleInteraction>> table;
    
        table[circleId][circleId] = [](Mephi::MephiManager& manager, size_t& moleculeInd1, size_t& moleculeInd2) {
            return manager.HandleInteractionCC_(moleculeInd1, moleculeInd2);
        };
        
        table[circleId][squareId] = [](Mephi::MephiManager& manager, size_t& moleculeInd1, size_t& moleculeInd2) {
            return manager.HandleInteractionCS_(moleculeInd1, moleculeInd2);
        };

        table[squareId][circleId] = [](Mephi::MephiManager& manager, size_t& moleculeInd1, size_t& moleculeInd2) {
            return manager.HandleInteractionCS_(moleculeInd2, moleculeInd1);
        };
        
        table[squareId][squareId] = [](Mephi::MephiManager& manager, size_t& moleculeInd1, size_t& moleculeInd2) {
            return manager.HandleInteractionSS_(moleculeInd1, moleculeInd2);
        };
    
    return std::move(table);
}();

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

    ERROR_HANDLE(HandleInteraction_());

    return Common::Error::SUCCESS;
}

Common::Error Mephi::MephiManager::HandleInteraction_() {

    // uint64_t sumMass = 0;
    for (size_t i = 0; i < molecules_.size(); ++i) {
        // sumMass += molecules_[i]->GetMass();
        for (size_t j = i + 1; j < molecules_.size(); ++j) {
            if (Mephi::IsIntersect(*molecules_[i], *molecules_[j])) {
                size_t id1 = molecules_[i]->GetTypeId();
                size_t id2 = molecules_[j]->GetTypeId();
                
                return handleIntercationFuncsTable_.at(id1).at(id2)(*this, i, j);
            }
        }
    }

    // std::cerr << "sumMass " << sumMass << std::endl;
    
    return Common::Error::SUCCESS;
}

Common::Error Mephi::MephiManager::HandleInteractionCC_(size_t& moleculeInd1, size_t& moleculeInd2) {
    const Mephi::Molecule& circle1 = *molecules_[moleculeInd1];
    const Mephi::Molecule& circle2 = *molecules_[moleculeInd2];

    
    molecules_[moleculeInd1] = std::make_unique<Mephi::MoleculeSquare>(Mephi::MoleculeSquare(
        Mephi::Vector2d((circle1.GetCoord() + circle2.GetCoord()) / 2.), 
        Mephi::Vector2d((circle1.GetSpeed() + circle2.GetSpeed()) / 2.),
        2,
        sf::Color::Blue
    ));
    
    std::swap(molecules_[moleculeInd2], molecules_.back());
    molecules_.pop_back();

    return Common::Error::SUCCESS;
}

Common::Error Mephi::MephiManager::HandleInteractionSS_(size_t& moleculeInd1, size_t& moleculeInd2) {
    const Mephi::Molecule& square1 = *molecules_[moleculeInd1];
    const Mephi::Molecule& square2 = *molecules_[moleculeInd2];

    const uint64_t sumMass = square1.GetMass() + square2.GetMass();
    const double speedModule = Mephi::Vector2d((square1.GetSpeed() + square2.GetSpeed()) / 2.).Len();

    srand(time(NULL));
    for (size_t i = 0; i < sumMass; ++i) {
        const double angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2 * std::numbers::pi;
        const double cos = std::cos(angle);
        const double sin = std::sin(angle);

        const Mephi::Vector2d avgSpeed(cos * speedModule - sin * speedModule, 
                                       sin * speedModule + cos * speedModule);

        const Mephi::Vector2d midPos((square1.GetCoord() + square2.GetCoord()) / 2.);

        molecules_.push_back(std::make_unique<Mephi::MoleculeCircle>(Mephi::MoleculeCircle(
            Mephi::Vector2d(midPos + avgSpeed * static_cast<double>(sumMass)),
            avgSpeed,
            sf::Color::Red
        )));
    }

    std::swap(molecules_[moleculeInd1], molecules_.back());
    molecules_.pop_back();
    std::swap(molecules_[moleculeInd2], molecules_.back());
    molecules_.pop_back();

    return Common::Error::SUCCESS;
}

Common::Error Mephi::MephiManager::HandleInteractionCS_(size_t& moleculeInd1, size_t& moleculeInd2) {    
    static_cast<Mephi::MoleculeSquare&>(*molecules_[moleculeInd2]).IncreaseMass(1);

    std::swap(molecules_[moleculeInd1], molecules_.back());
    molecules_.pop_back();
    
    return Common::Error::SUCCESS;
}

Common::Error Mephi::MephiManager::GenerateMolecules(const size_t count)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::uniform_real_distribution<double> distX(reactor_.GetLeftCorner().x, reactor_.GetRightCorner().x);
    std::uniform_real_distribution<double> distY(reactor_.GetLeftCorner().y, reactor_.GetRightCorner().y);
    
    std::uniform_real_distribution<double> distSpeed(0, 10);
    std::uniform_real_distribution<double> distAngle(0.0, 2.0 * M_PI);
    
    for (size_t i = 0; i < count; ++i)
    {
        const Mephi::Vector2d randomCoord(distX(gen), distY(gen));
        
        const double speed = distSpeed(gen);
        const double angle = distAngle(gen);
        const Mephi::Vector2d randomSpeed(speed * std::cos(angle), speed * std::sin(angle));
        
        const Mephi::MoleculeCircle molecule(randomCoord, randomSpeed, sf::Color::Red);
        
        molecules_.push_back(std::make_unique<Mephi::MoleculeCircle>(molecule));
    }

    return Common::Error::SUCCESS;
}