#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <chrono>
#include <numbers>

#include "windows/Clock.hpp"
#include "common/ErrorHandle.hpp"
#include "vector/Vector.hpp"

static std::tm* NowTime_() noexcept {
    auto now = std::chrono::system_clock::now();

    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

    return std::localtime(&nowTime);
}

static Mephi::Vector2d TimeVec_(const double time, const double vectorSize, const double divisions) {
    const double minAngle = 2 * std::numbers::pi / divisions;

    const double angle = time * minAngle;
    const double cos = std::cos(angle);
    const double sin = std::sin(angle);

    return {sin * vectorSize, cos * vectorSize};
}

static Mephi::Vector2d SecVec_(std::tm* nowTm, const double vectorSize) {
    return TimeVec_(nowTm->tm_sec, vectorSize, 60);
}

static Mephi::Vector2d MinVec_(std::tm* nowTm, const double vectorSize) {
    return TimeVec_(nowTm->tm_min + nowTm->tm_sec / 60., vectorSize, 60);
}

static Mephi::Vector2d HourVec_(std::tm* nowTm, const double vectorSize) {
    return TimeVec_(nowTm->tm_hour + nowTm->tm_min / 60., vectorSize, 24);
}

Common::Error Mephi::Clock::DrawArrows(sf::RenderWindow& window) const {
    std::tm* nowTm = NowTime_();

    const double vectorSize = std::min(rect_.Width(), rect_.Height()) / 2 - 4 * BIG_DOT_RADIUS;

    const Mephi::Vector2d secVec  (SecVec_ (nowTm, vectorSize));
    const Mephi::Vector2d minVec  (MinVec_ (nowTm, vectorSize * 0.8));
    const Mephi::Vector2d hourVec (HourVec_(nowTm, vectorSize * 0.6));

    const Mephi::Vector2d center(rect_.Width() / 2, rect_.Height() / 2);
    const Mephi::Vector2d origin = AbsoluteCoord() + center;

    ERROR_HANDLE(Mephi::DrawVector(window, origin, origin +  secVec.Mirror(Mephi::Axis::Y), arrowColor_));
    ERROR_HANDLE(Mephi::DrawVector(window, origin, origin +  minVec.Mirror(Mephi::Axis::Y), arrowColor_));
    ERROR_HANDLE(Mephi::DrawVector(window, origin, origin + hourVec.Mirror(Mephi::Axis::Y), arrowColor_));

    return Common::Error::SUCCESS;
}

Common::Error Mephi::Clock::DrawDots(sf::RenderWindow& window) const {
    const double radiusVectorSize = std::min(rect_.Width(), rect_.Height()) / 2 - 2 * BIG_DOT_RADIUS;

    const Mephi::Vector2d center(rect_.Width() / 2, rect_.Height() / 2);
    const Mephi::Vector2d origin = AbsoluteCoord() + center;

    sf::CircleShape circle(SMALL_DOT_RADIUS);
    circle.setFillColor(dotColor_);

    static constexpr size_t DIVISIONS = 60;
    static constexpr double MIN_ANGLE = 2 * std::numbers::pi / DIVISIONS;
    for (size_t dotNum = 0; dotNum < DIVISIONS; ++dotNum) {
        const double angle = dotNum * MIN_ANGLE;
        const double cos = std::cos(angle);
        const double sin = std::sin(angle);

        const Mephi::Vector2d radiusVector(sin * radiusVectorSize, cos * radiusVectorSize);
        const Mephi::Vector2d DotCenter = origin + radiusVector;

        Mephi::Vector2d circlePos = DotCenter - Mephi::Vector2d{SMALL_DOT_RADIUS, SMALL_DOT_RADIUS};
        double circleRadius = SMALL_DOT_RADIUS;

        if (dotNum % 5 == 0) {
            circlePos = DotCenter - Mephi::Vector2d{BIG_DOT_RADIUS, BIG_DOT_RADIUS};
            circleRadius = BIG_DOT_RADIUS;
        }

        circle.setPosition(static_cast<sf::Vector2f>(circlePos));
        circle.setRadius(circleRadius);

        window.draw(circle);
    }

    return Common::Error::SUCCESS;
}

Common::Error Mephi::Clock::DrawText(sf::RenderWindow& window) const {
    sf::Text printText = text_;
    printText.setPosition(static_cast<sf::Vector2f>(AbsoluteCoord()) + printText.getPosition());

    window.draw(printText);

    return Common::Error::SUCCESS;
}

Common::Error Mephi::Clock::UpdateText() {
    constexpr size_t TIME_STRING_SIZE_ = 80;
    char timeString_[TIME_STRING_SIZE_];

    std::tm* nowTm = NowTime_();

    std::strftime(timeString_, TIME_STRING_SIZE_, "%Y-%m-%d", nowTm);
    text_.setString(timeString_);

    sf::FloatRect textRect = text_.getLocalBounds();
    text_.setPosition(
        (rect_.Width()  - textRect.width)  / 2,
        (rect_.Height() - textRect.height) / 2
    );
    return Common::Error::SUCCESS;
}

Common::Error Mephi::Clock::Update() {
    ERROR_HANDLE(UpdateText());
    return Common::Error::SUCCESS;
}

Common::Error Mephi::Clock::Draw(sf::RenderWindow& window) const {
    Mephi::Window::Draw(window);

    ERROR_HANDLE(DrawDots(window));
    ERROR_HANDLE(DrawText(window));
    ERROR_HANDLE(DrawArrows(window));

    return Common::Error::SUCCESS;
}