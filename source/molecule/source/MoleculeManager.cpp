
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
        return Mephi::MoleculeManager::CIRCLE;
    }

    if (hash_code == typeid(Mephi::MoleculeSquare).hash_code()) {
        return Mephi::MoleculeManager::SQUARE;
    }

    return Mephi::MoleculeManager::UNKNOWN;
};

Common::Error Mephi::MoleculeManager::Draw(sf::RenderWindow& window) const {
    for (const auto& molecule : molecules_) {
        ERROR_HANDLE(molecule->Draw(window));
    }

    return Common::Error::SUCCESS;
}

Common::Error Mephi::MoleculeManager::HandleInteraction_() {
    for (size_t i = 0; i < molecules_.size(); ++i) {
        for (size_t j = i + 1; j < molecules_.size(); ++j) {
            if (Mephi::IsIntersect(*molecules_[i], *molecules_[j])) {
                Mephi::MoleculeManager::MoleculeType id1 = HashCode2MoleculeType(molecules_[i]->GetTypeId());
                Mephi::MoleculeManager::MoleculeType id2 = HashCode2MoleculeType(molecules_[j]->GetTypeId());

                if (id2 > id1)  { ERROR_HANDLE((this->*handleIntercationFuncsTable_[id1][id2])(i, j)); }
                else            { ERROR_HANDLE((this->*handleIntercationFuncsTable_[id1][id2])(j, i)); }
            }
        }
    }
    
    return Common::Error::SUCCESS;
}

Common::Error Mephi::MoleculeManager::HandleInteractionCC_(size_t moleculeInd1, size_t moleculeInd2) {
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

    circleCnt_ -= 2;
    squareCnt_ += 1;

    return Common::Error::SUCCESS;
}

Common::Error Mephi::MoleculeManager::HandleInteractionSS_(size_t moleculeInd1, size_t moleculeInd2) {
    const Mephi::Molecule& square1 = *molecules_[moleculeInd1];
    const Mephi::Molecule& square2 = *molecules_[moleculeInd2];
    const uint64_t sumMass = square1.GetMass() + square2.GetMass();

    const Mephi::Vector2d centerMassSpeed((
            static_cast<double>(square1.GetMass()) * square1.GetSpeed() 
          + static_cast<double>(square2.GetMass()) * square2.GetSpeed()
        ) / static_cast<double>(sumMass)
    );

    const double speedModule = sqrt((square1.GetMass() * square1.GetSpeed().Len2() + square2.GetMass() * square2.GetSpeed().Len2()) / sumMass);

    srand(time(NULL));
    for (size_t i = 0; i < sumMass; ++i) {
        const double angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2 * std::numbers::pi;
        const double cos = std::cos(angle);
        const double sin = std::sin(angle);

        const Mephi::Vector2d relSpeed(cos * speedModule, sin * speedModule);

        const uint64_t sizeSquare = Mephi::MoleculeSquare::START_RADIUS + sumMass;
        const Mephi::Vector2d addPos(cos * sizeSquare - sin * sizeSquare,
                                     sin * sizeSquare + cos * sizeSquare);

        const Mephi::Vector2d midPos((square1.GetCoord() + square2.GetCoord()) / 2.);

        molecules_.push_back(std::make_unique<Mephi::MoleculeCircle>(Mephi::MoleculeCircle(
            Mephi::Vector2d(midPos + addPos),
            Mephi::Vector2d(centerMassSpeed + relSpeed),
            sf::Color::Red
        )));
    }

    circleCnt_ += sumMass;
    squareCnt_ -= 2;

    std::swap(molecules_[moleculeInd1], molecules_.back());
    molecules_.pop_back();
    std::swap(molecules_[moleculeInd2], molecules_.back());
    molecules_.pop_back();

    return Common::Error::SUCCESS;
}

Common::Error Mephi::MoleculeManager::HandleInteractionCS_(size_t moleculeInd1, size_t moleculeInd2) {  
    const Mephi::Molecule& circle = *molecules_[moleculeInd1];
    const Mephi::Molecule& square = *molecules_[moleculeInd2];

    static_cast<Mephi::MoleculeSquare&>(*molecules_[moleculeInd2]).GetSpeed() = 
        Mephi::Vector2d((circle.GetSpeed() + (double)square.GetMass() * square.GetSpeed()) / double(square.GetMass() + 1));
    static_cast<Mephi::MoleculeSquare&>(*molecules_[moleculeInd2]).IncreaseMass(1);

    std::swap(molecules_[moleculeInd1], molecules_.back());
    molecules_.pop_back();

    --circleCnt_;
    
    return Common::Error::SUCCESS;
}

Common::Error Mephi::MoleculeManager::GenerateMolecules(const size_t count, const Mephi::Rect& rect)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    circleCnt_ += count;
    
    std::uniform_real_distribution<double> distX(rect.GetLeftCorner().x, rect.GetRightCorner().x);
    std::uniform_real_distribution<double> distY(rect.GetLeftCorner().y, rect.GetRightCorner().y);
    
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

double Mephi::MoleculeManager::TotalEnergy() const noexcept{
    double energy = 0;
    for (const auto& molecule : molecules_) {
        energy += 0.5 * molecule->GetMass() * molecule->GetSpeed().Len2();
    }
    return energy;
}