#include "windows/buttons/DeleteMoleculesButton.hpp"
#include "common/ErrorHandle.hpp"
#include "windows/Reactor.hpp"

Common::Error Mephi::DeleteMoleculesButton::Update() {
    if (isPressed_) {
        const size_t moleculesCnt = reactor_->GetMoleculeManager().GetMoleculesCnt();
        ERROR_HANDLE(reactor_->DeleteMolecules(std::min(moleculesCnt, count_)));
    }

    ERROR_HANDLE(Mephi::Button::Update());

    return Common::Error::SUCCESS;
}


