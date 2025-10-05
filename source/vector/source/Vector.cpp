#include <vector/Vector.hpp>

Common::Error Mephi::TransformVector(Mephi::Vector2f& Vector, const Mephi::Transform Transform,
                                     const float AngleRadians) {
    float RotateAngleRadians = NAN;

    switch (Transform) {
        case Mephi::Transform::ROTATE_CLKWISE:  RotateAngleRadians = AngleRadians; break;
        case Mephi::Transform::ROTATE_CCLKWISE: RotateAngleRadians = AngleRadians; break;
        case Mephi::Transform::NONE:            return Common::Error::SUCCESS;
        default:                                return Common::Error::UNKNOWN_ENUM_ARG;
    }

    float cosA = std::cos(RotateAngleRadians);
    float sinA = std::sin(RotateAngleRadians);

    Vector = Mephi::Vector2f(
        Vector.x * cosA - Vector.y * sinA,
        Vector.x * sinA + Vector.y * cosA
    );

    return Common::Error::SUCCESS;
}

Common::Error Mephi::TransformVector(Mephi::Vector3f& Vector, const Mephi::Transform Transform, 
                                     Mephi::Axis Axis, const float AngleRadians) {
    float RotateAngleRadians = NAN;

    switch (Transform) {
        case Mephi::Transform::ROTATE_CLKWISE:  RotateAngleRadians = AngleRadians; break;
        case Mephi::Transform::ROTATE_CCLKWISE: RotateAngleRadians = AngleRadians; break;
        case Mephi::Transform::NONE:            return Common::Error::SUCCESS;
        default:                                   return Common::Error::UNKNOWN_ENUM_ARG;
    }

    float cosA = std::cos(RotateAngleRadians);
    float sinA = std::sin(RotateAngleRadians);
    float x = Vector.x, y = Vector.y, z = Vector.z;

    switch (Axis) {
        case Mephi::Axis::X:
            Vector = Mephi::Vector3f(
                x,
                y * cosA - z * sinA,
                y * sinA + z * cosA
            );
            break;

        case Mephi::Axis::Y:
            Vector = Mephi::Vector3f(
                x * cosA + z * sinA,
                y,
                -x * sinA + z * cosA
            );
            break;

        case Mephi::Axis::Z:
            Vector = Mephi::Vector3f(
                x * cosA - y * sinA,
                x * sinA + y * cosA,
                z
            );
            break;

        default:
            return Common::Error::UNKNOWN_ENUM_ARG;
    }

    return Common::Error::SUCCESS;
}

Common::Error Mephi::DrawVector(sf::RenderWindow& window, Mephi::Vector2d startDot, Mephi::Vector2d endDot,
                                sf::Color color) {

    const Mephi::Vector2d vector(endDot.x - startDot.x, 
                                 endDot.y - startDot.y);

    sf::VertexArray Line(sf::PrimitiveType::LinesStrip, 2);
    sf::VertexArray Tip (sf::PrimitiveType::LinesStrip, 3);

    Line[0].color = Line[1].color                = color;
    Tip[0].color  = Tip[1].color  = Tip[2].color = color;

    Line[0].position = static_cast<sf::Vector2f>(startDot);
    Line[1].position = static_cast<sf::Vector2f>(endDot);
    Tip[1].position  = Line[1].position;

    constexpr double TIP_SCALE = 0.1;

    const Mephi::Vector2d ScaledTendril(-vector * TIP_SCALE);
    const Mephi::Vector2d NormalTendril(ScaledTendril.Normal());
    const Mephi::Vector2d    TanTendril(vector + 2. * ScaledTendril);

    Tip[0].position = static_cast<sf::Vector2f>(TanTendril + NormalTendril);
    Tip[2].position = static_cast<sf::Vector2f>(TanTendril - NormalTendril);

    Tip[0].position = static_cast<sf::Vector2f>(startDot + Mephi::Vector2d(Tip[0].position));
    Tip[2].position = static_cast<sf::Vector2f>(startDot + Mephi::Vector2d(Tip[2].position));

    window.draw(Line);
    window.draw(Tip);
    
    return Common::Error::SUCCESS;
}