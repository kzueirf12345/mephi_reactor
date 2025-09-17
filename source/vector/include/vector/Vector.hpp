#ifndef MEPHI_REACTOR_SOURCE_VECTOR_INCLUDE_VECTOR_VECTOR_HPP
#define MEPHI_REACTOR_SOURCE_VECTOR_INCLUDE_VECTOR_VECTOR_HPP

#include <cmath>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include "common/ErrorHandle.hpp"

namespace Mephi
{

enum class Axis {
    X = 0,
    Y = 1,
    Z = 2
};

enum Transform {
    NONE            = 0,
    ROTATE_CLKWISE  = 1,
    ROTATE_CCLKWISE = 2,
};

//--------------------------------------------------------------------------------------------------

template <typename T, template<typename> typename ParentT = sf::Vector2>
class Vector2: public ParentT<T> {
    private:
    public:
        Vector2() : ParentT<T>() {}
        Vector2(T X, T Y) : ParentT<T>(X, Y) {}
        template <typename U>
        explicit Vector2(const ParentT<T>& vector)  : ParentT<T>(vector) {}
        explicit Vector2(const ParentT<T>&& vector) : ParentT<T>(vector) {}

        [[nodiscard]] T Len2() const;
        [[nodiscard]] T Len()  const;
        [[nodiscard]] Vector2<T, ParentT> Normal() const;
};

typedef Mephi::Vector2<float>  Vector2f;
typedef Mephi::Vector2<double> Vector2d;
typedef Mephi::Vector2<int>    Vector2i;

template <typename T, template<typename> typename ParentT = sf::Vector2>
T operator ^(const Mephi::Vector2<T, ParentT>& left, const Mephi::Vector2<T, ParentT>& right) {
    return left.x * right.x + left.y * right.y;
}

template <typename T, template<typename> typename ParentT = sf::Vector2>
Mephi::Vector2<T, ParentT> operator !(const Mephi::Vector2<T, ParentT>& right) {
    return Mephi::Vector2<T, ParentT>(right / right.Len());
}

template <typename T, template<typename> typename ParentT>
Vector2<T, ParentT>& operator+=(Vector2<T, ParentT>& left, const Vector2<T, ParentT>& right) {
    left.x += right.x;
    left.y += right.y;
    return left;
}

template <typename T, template<typename> typename ParentT>
T Mephi::Vector2<T, ParentT>::Len2() const {
    return (*this) ^ (*this);
}

template <typename T, template<typename> typename ParentT>
T Mephi::Vector2<T, ParentT>::Len() const {
    return std::sqrt(this->Len2());
}

template <typename T, template<typename> typename ParentT>
Mephi::Vector2<T, ParentT> Mephi::Vector2<T, ParentT>::Normal() const {
    return Vector2<T, ParentT>(this->y, -this->x);
}\

Common::Error TransformVector(Mephi::Vector2f& Vector, const Mephi::Transform Transform, 
                              const float AngleRadians = 0.01);

//--------------------------------------------------------------------------------------------------

template <typename T, template<typename> typename ParentT = sf::Vector3>
class Vector3: public ParentT<T> {
    private:
    public:
        Vector3() : ParentT<T>() {}
        Vector3(T X, T Y, T Z) : ParentT<T>(X, Y, Z) {};
        template <typename U>
        explicit Vector3(const ParentT<T>&  vector) : ParentT<T>(vector) {}
        explicit Vector3(const ParentT<T>&& vector) : ParentT<T>(vector) {}
        
        [[nodiscard]] T Len2() const;
        [[nodiscard]] T Len()  const;
};

typedef Mephi::Vector3<float>  Vector3f;
typedef Mephi::Vector2<double> Vector3d;
typedef Mephi::Vector3<int>    Vector3i;

template <typename T, template<typename> typename ParentT = sf::Vector2>
T operator ^(const Mephi::Vector3<T, ParentT>& left, const Mephi::Vector3<T, ParentT>& right) {
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

template <typename T, template<typename> typename ParentT = sf::Vector2>
Mephi::Vector3<T, ParentT> operator !(const Mephi::Vector3<T, ParentT>& right) {
    return Mephi::Vector3<T, ParentT>(right / right.Len());
}

template <typename T, template<typename> typename ParentT>
T Mephi::Vector3<T, ParentT>::Len2() const {
    return (*this) ^ (*this);
}

template <typename T, template<typename> typename ParentT>
T Mephi::Vector3<T, ParentT>::Len() const {
    return std::sqrt(this->Len2());
}

Common::Error TransformVector(Mephi::Vector3f& Vector, const Mephi::Transform Transform, 
                              Mephi::Axis Axis, const float AngleRadians = 0.01);

}

#endif /*MEPHI_REACTOR_SOURCE_VECTOR_INCLUDE_VECTOR_VECTOR_HPP*/
