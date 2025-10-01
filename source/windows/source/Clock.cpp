#include <chrono>

#include "windows/Clock.hpp"

Common::Error Mephi::Clock::Update() {
    constexpr size_t TIME_STRING_SIZE_ = 80;
    char timeString_[TIME_STRING_SIZE_];
    auto now = std::chrono::system_clock::now();

    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::tm* now_tm = std::localtime(&now_c);

    std::strftime(timeString_, TIME_STRING_SIZE_, "%H:%M:%S", now_tm);
    text_.setString(timeString_);

    sf::FloatRect textRect = text_.getLocalBounds();
    text_.setPosition(
        (rect_.Width()  - textRect.width)  / 2,
        (rect_.Height() - textRect.height) / 2
    );
    return Common::Error::SUCCESS;
}

Common::Error Mephi::Clock::Draw(sf::RenderWindow& window) const {
    Mephi::Window::Draw(window);

    sf::Text printText = text_;
    printText.setPosition(static_cast<sf::Vector2f>(AbsoluteCoord()) + printText.getPosition());

    window.draw(printText);
    return Common::Error::SUCCESS;
}