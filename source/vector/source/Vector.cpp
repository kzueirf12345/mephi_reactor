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