#include <typeinfo>

#include "molecule/Molecule.hpp"

size_t Mephi::Molecule::GetTypeId() const noexcept{
    return typeid(*this).hash_code();
}

bool Mephi::IsIntersect(const Mephi::Molecule& molecule1, 
                        const Mephi::Molecule& molecule2) noexcept {
               
    const double r1 = molecule1.GetRadius();
    const double r2 = molecule2.GetRadius();
    const double rDiff = std::abs(r2 - r1);
    const double rSum = r1 + r2;

    const double dist = (molecule1.GetCoord() - molecule2.GetCoord()).Len2();
    
    return rDiff * rDiff <= dist && dist <= rSum * rSum;
}

Common::Error Mephi::Molecule::Update(){
    coord_ += speed_;
    return Common::Error::SUCCESS;
}