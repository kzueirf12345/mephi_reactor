
#include "common/ErrorHandle.hpp"
#include "mephi/MephiManager.hpp"

Common::Error Mephi::MephiManager::Draw(sf::RenderWindow& window) const {

    reactor_.Draw(window);

    return Common::Error::SUCCESS;
}