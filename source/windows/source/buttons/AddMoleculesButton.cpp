#include "windows/buttons/AddMoleculesButton.hpp"
#include "common/ErrorHandle.hpp"
#include "windows/Reactor.hpp"

Common::Error Mephi::AddMoleculesButton::Update() {
    if (isPressed_) {
        ERROR_HANDLE(reactor_->GenerateMolecules(count_, maxSpeed_));
    }

    ERROR_HANDLE(Mephi::Button::Update());

    return Common::Error::SUCCESS;
}


