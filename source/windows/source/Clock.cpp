#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <chrono>
#include <numbers>

#include "windows/Clock.hpp"
#include "common/ErrorHandle.hpp"
#include "vector/Vector.hpp"

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

// WRITE VECTOR DEBUG

    auto now = std::chrono::system_clock::now();

    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::tm* now_tm = std::localtime(&now_c);

    const double vectorSize = std::min(rect_.Width(), rect_.Height()) / 2;

    const double secMinAngle = 2 * std::numbers::pi / 60;
    const double secAngle = -now_tm->tm_sec * secMinAngle;
    const double secCos = std::cos(secAngle);
    const double secSin = std::sin(secAngle);
    const double minMinAngle = 2 * std::numbers::pi / 60;
    const double minAngle = -now_tm->tm_min * minMinAngle;
    const double minCos = std::cos(minAngle);
    const double minSin = std::sin(minAngle);
    const double hourMinAngle = 2 * std::numbers::pi / 24;
    const double hourAngle = -now_tm->tm_hour * hourMinAngle;
    const double hourCos = std::cos(hourAngle);
    const double hourSin = std::sin(hourAngle);

    const Mephi::Vector2d startVec(0, vectorSize);
    const Mephi::Vector2d secVec (secCos * startVec.x - secSin * startVec.y,
                                  secSin * startVec.x + secCos * startVec.y);
    const Mephi::Vector2d minVec (minCos * startVec.x - minSin * startVec.y,
                                  minSin * startVec.x + minCos * startVec.y);
    const Mephi::Vector2d hourVec(hourCos * startVec.x - hourSin * startVec.y,
                                  hourSin * startVec.x + hourCos * startVec.y);

    const Mephi::Vector2d center(rect_.Width() / 2, rect_.Height() / 2);
    const Mephi::Vector2d origin = AbsoluteCoord() + center;


    ERROR_HANDLE(Mephi::DrawVector(window, origin, origin +  secVec.Mirror(Mephi::Axis::Y), sf::Color::Green));
    ERROR_HANDLE(Mephi::DrawVector(window, origin, origin +  minVec.Mirror(Mephi::Axis::Y), sf::Color::Green));
    ERROR_HANDLE(Mephi::DrawVector(window, origin, origin + hourVec.Mirror(Mephi::Axis::Y), sf::Color::Green));


// WRITE VECTOR DEBUG

    return Common::Error::SUCCESS;
}

// Common::Error Mephi::Clock::DrawVector(sf::RenderWindow& window, Mephi::Vector2d vector,
//                                        sf::Color color) const {

//     vector = {vector.x, -vector.y};
//     const Mephi::Vector2d center(rect_.Width() / 2, rect_.Height() / 2);

//     sf::VertexArray Line(sf::PrimitiveType::LinesStrip, 2);
//     sf::VertexArray Tip (sf::PrimitiveType::LinesStrip, 3);

//     Line[0].color = Line[1].color                = color;
//     Tip[0].color  = Tip[1].color  = Tip[2].color = color;

//     Line[0].position = static_cast<sf::Vector2f>(AbsoluteCoord() + center);
//     Line[1].position = static_cast<sf::Vector2f>(AbsoluteCoord() + center + vector);
//     Tip[1].position  = Line[1].position;

//     constexpr double TIP_SCALE = 0.1;

//     const Mephi::Vector2d ScaledTendril(-vector * TIP_SCALE);
//     const Mephi::Vector2d NormalTendril(ScaledTendril.Normal());
//     const Mephi::Vector2d    TanTendril(vector + 2. * ScaledTendril);

//     Tip[0].position = static_cast<sf::Vector2f>(TanTendril + NormalTendril);
//     Tip[2].position = static_cast<sf::Vector2f>(TanTendril - NormalTendril);

//     Tip[0].position = static_cast<sf::Vector2f>(
//         AbsoluteCoord() + center + Mephi::Vector2d(Tip[0].position)
//     );
//     Tip[2].position = static_cast<sf::Vector2f>(
//         AbsoluteCoord() + center + Mephi::Vector2d(Tip[2].position)
//     );

//     window.draw(Line);
//     window.draw(Tip);
    
//     return Common::Error::SUCCESS;
// }