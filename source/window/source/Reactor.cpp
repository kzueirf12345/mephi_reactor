#include "window/Reactor.hpp"
#include "common/ErrorHandle.hpp"

Common::Error Mephi::Reactor::Draw(sf::RenderWindow& window) const {
    for (auto& child : children_) {
        ERROR_HANDLE(child->Draw(window));
    }

    ERROR_HANDLE(rect_.Draw(window));
    return Common::Error::SUCCESS;
}