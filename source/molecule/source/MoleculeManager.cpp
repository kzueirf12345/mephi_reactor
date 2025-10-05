
#include <cstddef>
#include <memory>
#include <random>

#include "common/ErrorHandle.hpp"
#include "molecule/Molecule.hpp"
#include "molecule/MoleculeCircle.hpp"
#include "molecule/MoleculeSquare.hpp"
#include "vector/Vector.hpp"
#include "molecule/MoleculeManager.hpp"

const Mephi::MoleculeManager::FHandleInteraction 
    Mephi::MoleculeManager::handleIntercationFuncsTable_
        [Mephi::MoleculeManager::MoleculeType::SIZE]
        [Mephi::MoleculeManager::MoleculeType::SIZE] = {
    {
        &Mephi::MoleculeManager::HandleInteractionCC_,
        &Mephi::MoleculeManager::HandleInteractionCS_
    },
    {
        &Mephi::MoleculeManager::HandleInteractionCS_,
        &Mephi::MoleculeManager::HandleInteractionSS_
    }
};

Mephi::MoleculeManager::MoleculeType Mephi::MoleculeManager::HashCode2MoleculeType(const size_t hash_code) {

    if (hash_code == typeid(Mephi::MoleculeCircle).hash_code()) {
        return Mephi::MoleculeManager::MoleculeType::CIRCLE;
    }

    if (hash_code == typeid(Mephi::MoleculeSquare).hash_code()) {
        return Mephi::MoleculeManager::MoleculeType::SQUARE;
    }

    return Mephi::MoleculeManager::MoleculeType::UNKNOWN;
};

Common::Error Mephi::MoleculeManager::Draw(sf::RenderWindow& window) const {
    for (const auto& molecule : molecules_) {
        ERROR_HANDLE(molecule->Draw(window));
    }

    return Common::Error::SUCCESS;
}

Common::Error Mephi::MoleculeManager::HandleInteraction_() {
    std::vector<bool> toRemove(molecules_.size(), false);
    
    for (size_t i = 0; i < molecules_.size(); ++i) {
        if (toRemove[i]) continue;
        
        for (size_t j = i + 1; j < molecules_.size(); ++j) {
            if (toRemove[j] || toRemove[i]) continue;
            
            if (Mephi::IsIntersect(*molecules_[i], *molecules_[j])) {
                Mephi::MoleculeManager::MoleculeType id1 = HashCode2MoleculeType(molecules_[i].get()->GetTypeId());
                Mephi::MoleculeManager::MoleculeType id2 = HashCode2MoleculeType(molecules_[j].get()->GetTypeId());

                (this->*handleIntercationFuncsTable_[id1][id2])(i, j, toRemove);
            }
        }
    }
    
    size_t writeIndex = 0;
    for (size_t readIndex = 0; readIndex < molecules_.size(); ++readIndex) {
        if (!toRemove[readIndex]) {
            if (readIndex != writeIndex) {
                molecules_[writeIndex] = std::move(molecules_[readIndex]);

            }
                ++writeIndex;
        }
    }
    molecules_.resize(writeIndex);
    
    return Common::Error::SUCCESS;
}

Common::Error Mephi::MoleculeManager::HandleInteractionCC_(size_t moleculeInd1, size_t moleculeInd2, std::vector<bool>& toRemove) {
    const Mephi::Molecule& circle1 = *molecules_[moleculeInd1];
    const Mephi::Molecule& circle2 = *molecules_[moleculeInd2];


    molecules_.push_back(std::make_unique<Mephi::MoleculeSquare>(
        (circle1.GetCoord() + circle2.GetCoord()) / 2., 
        (circle1.GetSpeed() + circle2.GetSpeed()) / 2.,
        2,
        sf::Color::Blue
    ));
    toRemove.push_back(false);
    
    toRemove[moleculeInd1] = true;
    toRemove[moleculeInd2] = true;

    circleCnt_ -= 2;
    squareCnt_ += 1;

    return Common::Error::SUCCESS;
}

Common::Error Mephi::MoleculeManager::HandleInteractionSS_(size_t moleculeInd1, size_t moleculeInd2, std::vector<bool>& toRemove) {
    const Mephi::Molecule& square1 = *molecules_[moleculeInd1];
    const Mephi::Molecule& square2 = *molecules_[moleculeInd2];
    const double sumMass = square1.GetMass() + square2.GetMass();

    const Mephi::Vector2d centerMassSpeed((
            square1.GetMass() * square1.GetSpeed() 
          + square2.GetMass() * square2.GetSpeed()
        ) / sumMass
    );

    const double speedModule = sqrt((
            square1.GetMass() * square1.GetSpeed().Len2() 
          + square2.GetMass() * square2.GetSpeed().Len2()
        ) / sumMass
    );

    const Mephi::Vector2d midPos((square1.GetCoord() + square2.GetCoord()) / 2.);

    srand(time(NULL));
    for (size_t i = 0; i < sumMass; ++i) {
        const double angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2 * std::numbers::pi;
        const double cos = std::cos(angle);
        const double sin = std::sin(angle);

        const Mephi::Vector2d relSpeed(cos * speedModule, sin * speedModule);

        const double sizeSquare = Mephi::MoleculeSquare::START_RADIUS + sumMass;
        const Mephi::Vector2d addPos(cos * sizeSquare - sin * sizeSquare,
                                     sin * sizeSquare + cos * sizeSquare);

        molecules_.push_back(std::make_unique<Mephi::MoleculeCircle>(
            midPos + addPos,
            centerMassSpeed + relSpeed,
            sf::Color::Red
        ));
        toRemove.push_back(false);
    }

    circleCnt_ += sumMass;
    squareCnt_ -= 2;

    toRemove[moleculeInd1] = true;
    toRemove[moleculeInd2] = true;

    return Common::Error::SUCCESS;
}

Common::Error Mephi::MoleculeManager::HandleInteractionCS_(size_t moleculeInd1, size_t moleculeInd2, std::vector<bool>& toRemove) {  
    size_t circleInd = moleculeInd1;
    size_t squareInd = moleculeInd2;

    if (!dynamic_cast<MoleculeCircle*>(molecules_[moleculeInd1].get())) {
        std::swap(circleInd, squareInd);
    }

    auto& circle = *dynamic_cast<Mephi::MoleculeCircle*>(molecules_[circleInd].get());
    auto& square = *dynamic_cast<Mephi::MoleculeSquare*>(molecules_[squareInd].get());

    square.GetSpeed() = (
            circle.GetSpeed() 
          + square.GetMass() * square.GetSpeed()
        ) / (square.GetMass() + 1);
        
    square.IncreaseMass(1);

    toRemove[circleInd] = true;

    --circleCnt_;
    
    return Common::Error::SUCCESS;
}

Common::Error Mephi::MoleculeManager::GenerateMolecules(const size_t count, const double maxSpeed, 
                                                        const Mephi::Rect& rect) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    circleCnt_ += count;
    
    std::uniform_real_distribution<double> distX(
        rect.GetLeftCorner() .x + Mephi::MoleculeCircle::START_RADIUS,
        rect.GetRightCorner().x - Mephi::MoleculeCircle::START_RADIUS);
    std::uniform_real_distribution<double> distY(
        rect.GetLeftCorner() .y + Mephi::MoleculeCircle::START_RADIUS, 
        rect.GetRightCorner().y - Mephi::MoleculeCircle::START_RADIUS);
    
    std::uniform_real_distribution<double> distSpeed(0, maxSpeed);
    std::uniform_real_distribution<double> distAngle(0.0, 2 * std::numbers::pi);
    
    for (size_t i = 0; i < count; ++i) {
        const Mephi::Vector2d randomCoord(distX(gen), distY(gen));
        
        const double speed = distSpeed(gen);
        const double angle = distAngle(gen);
        const Mephi::Vector2d randomSpeed(speed * std::cos(angle), speed * std::sin(angle));
        
        const Mephi::MoleculeCircle molecule(randomCoord, randomSpeed, sf::Color::Red);
        
        molecules_.push_back(std::make_unique<Mephi::MoleculeCircle>(molecule));
    }

    return Common::Error::SUCCESS;
}

Common::Error Mephi::MoleculeManager::DeleteMolecules(const size_t count) {
    if (count <= molecules_.size())
    {
        size_t removedSquareCnt = 0;
        const auto it = molecules_.rbegin();
        for (size_t i = 0; i < count; ++i) {
            removedSquareCnt += (dynamic_cast<Mephi::MoleculeSquare*>((*(molecules_.rbegin() + i)).get()) != NULL);
        }
        circleCnt_ -= count - removedSquareCnt;
        squareCnt_ -= removedSquareCnt;

        molecules_.resize(molecules_.size() - count);
    }

    return Common::Error::SUCCESS;
}

double Mephi::MoleculeManager::TotalEnergy() const noexcept{
    double energy = 0;
    for (const auto& molecule : molecules_) {
        energy += molecule->KinEnergy();
    }
    return energy;
}

double Mephi::MoleculeManager::TotalMass() const noexcept{
    double mass = 0;
    for (const auto& molecule : molecules_) {
        mass += molecule->GetMass();
    }
    return mass;
}