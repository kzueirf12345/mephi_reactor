#include <typeinfo>

#include "molecule/Molecule.hpp"

size_t Mephi::Molecule::GetTypeId() const noexcept{
    return typeid(*this).hash_code();
}

bool Mephi::IsIntersect(const Mephi::Molecule& molecule1, 
                        const Mephi::Molecule& molecule2) noexcept {
                            
    bool xOverlap = molecule1.LeftX()  <= molecule2.RightX() && 
                    molecule1.RightX() >= molecule2.LeftX();
    
    bool yOverlap = molecule1.TopY()    <= molecule2.BottomY() && 
                    molecule1.BottomY() >= molecule2.TopY();
    
    return xOverlap && yOverlap;
}