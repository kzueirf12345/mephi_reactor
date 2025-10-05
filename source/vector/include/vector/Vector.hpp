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
        explicit Vector2(const ParentT<U>& vector)  : ParentT<T>(vector) {}
        explicit Vector2(const ParentT<T>& vector)  : ParentT<T>(vector) {}
        explicit Vector2(const ParentT<T>&& vector) : ParentT<T>(vector) {}

        [[nodiscard]] T Len2() const;
        [[nodiscard]] T Len()  const;
        [[nodiscard]] Vector2<T, ParentT> Normal() const;
        [[nodiscard]] Vector2<T, ParentT> Mirror(Mephi::Axis axis) const;
        [[nodiscard]] Vector2<T, ParentT> Clump(const Mephi::Vector2<T, ParentT>& minVec, 
                                                const Mephi::Vector2<T, ParentT>& maxVec) const;

        Vector2<T, ParentT> operator-() const {
            return Vector2<T, ParentT>(-this->x, -this->y);
        }

        Vector2<T, ParentT>& operator+=(const Vector2<T, ParentT>& other) {
            this->x += other.x;
            this->y += other.y;
            return *this;
        }

        Vector2<T, ParentT>& operator-=(const Vector2<T, ParentT>& other) {
            this->x -= other.x;
            this->y -= other.y;
            return *this;
        }

        Vector2<T, ParentT>& operator*=(T scalar) {
            this->x *= scalar;
            this->y *= scalar;
            return *this;
        }

        Vector2<T, ParentT>& operator/=(T scalar) {
            this->x /= scalar;
            this->y /= scalar;
            return *this;
        }
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
Vector2<T, ParentT> operator+(const Vector2<T, ParentT>& left, const Vector2<T, ParentT>& right) {
    return Vector2<T, ParentT>(left.x + right.x, left.y + right.y);
}

template <typename T, template<typename> typename ParentT>
Vector2<T, ParentT> operator-(const Vector2<T, ParentT>& left, const Vector2<T, ParentT>& right) {
    return Vector2<T, ParentT>(left.x - right.x, left.y - right.y);
}

template <typename T, template<typename> typename ParentT>
Vector2<T, ParentT> operator*(const Vector2<T, ParentT>& left, T right) {
    return Vector2<T, ParentT>(left.x * right, left.y * right);
}

template <typename T, template<typename> typename ParentT>
Vector2<T, ParentT> operator*(T left, const Vector2<T, ParentT>& right) {
    return Vector2<T, ParentT>(right.x * left, right.y * left);
}

template <typename T, template<typename> typename ParentT>
Vector2<T, ParentT> operator/(const Vector2<T, ParentT>& left, T right) {
    return Vector2<T, ParentT>(left.x / right, left.y / right);
}

template <typename T, template<typename> typename ParentT>
bool operator==(const Vector2<T, ParentT>& left, const Vector2<T, ParentT>& right) {
    return left.x == right.x && left.y == right.y;
}

template <typename T, template<typename> typename ParentT>
bool operator!=(const Vector2<T, ParentT>& left, const Vector2<T, ParentT>& right) {
    return !(left == right);
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
}

template <typename T, template<typename> typename ParentT>
Vector2<T, ParentT> Mephi::Vector2<T, ParentT>::Mirror(Mephi::Axis axis) const {
    return {
        axis == Mephi::Axis::X ? -this->x : this->x,
        axis == Mephi::Axis::Y ? -this->y : this->y
    };
}

template <typename T, template<typename> typename ParentT>
Vector2<T, ParentT> Mephi::Vector2<T, ParentT>::Clump(const Mephi::Vector2<T, ParentT>& minVec, 
                                                      const Mephi::Vector2<T, ParentT>& maxVec) const {
    return {
        std::max(std::min(this->x, maxVec.x), minVec.x),
        std::max(std::min(this->y, maxVec.y), minVec.y)
    };
}

Common::Error TransformVector(Mephi::Vector2f& Vector, const Mephi::Transform Transform, 
                              const float AngleRadians = 0.01);

Common::Error DrawVector(sf::RenderWindow& window, Mephi::Vector2d startDot, Mephi::Vector2d endDot,
                         sf::Color color);

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
        [[nodiscard]] Vector3<T, ParentT> Mirror(Mephi::Axis axis) const;
        [[nodiscard]] Vector3<T, ParentT> Clump(const Mephi::Vector3<T, ParentT>& minVec, 
                                                const Mephi::Vector3<T, ParentT>& maxVec) const;

        Vector3<T, ParentT> operator-() const {
            return Vector3<T, ParentT>(-this->x, -this->y, -this->z);
        }

        Vector3<T, ParentT>& operator+=(const Vector3<T, ParentT>& other) {
            this->x += other.x;
            this->y += other.y;
            this->z += other.z;
            return *this;
        }

        Vector3<T, ParentT>& operator-=(const Vector3<T, ParentT>& other) {
            this->x -= other.x;
            this->y -= other.y;
            this->z -= other.z;
            return *this;
        }

        Vector3<T, ParentT>& operator*=(T scalar) {
            this->x *= scalar;
            this->y *= scalar;
            this->z *= scalar;
            return *this;
        }

        Vector3<T, ParentT>& operator/=(T scalar) {
            this->x /= scalar;
            this->y /= scalar;
            this->z /= scalar;
            return *this;
        }
};

typedef Mephi::Vector3<float>  Vector3f;
typedef Mephi::Vector3<double> Vector3d;
typedef Mephi::Vector3<int>    Vector3i;

template <typename T, template<typename> typename ParentT = sf::Vector3>
T operator ^(const Mephi::Vector3<T, ParentT>& left, const Mephi::Vector3<T, ParentT>& right) {
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

template <typename T, template<typename> typename ParentT = sf::Vector3>
Mephi::Vector3<T, ParentT> operator !(const Mephi::Vector3<T, ParentT>& right) {
    return Mephi::Vector3<T, ParentT>(right / right.Len());
}

template <typename T, template<typename> typename ParentT>
Vector3<T, ParentT> operator+(const Vector3<T, ParentT>& left, const Vector3<T, ParentT>& right) {
    return Vector3<T, ParentT>(left.x + right.x, left.y + right.y, left.z + right.z);
}

template <typename T, template<typename> typename ParentT>
Vector3<T, ParentT> operator-(const Vector3<T, ParentT>& left, const Vector3<T, ParentT>& right) {
    return Vector3<T, ParentT>(left.x - right.x, left.y - right.y, left.z - right.z);
}

template <typename T, template<typename> typename ParentT>
Vector3<T, ParentT> operator*(const Vector3<T, ParentT>& left, T right) {
    return Vector3<T, ParentT>(left.x * right, left.y * right, left.z * right);
}

template <typename T, template<typename> typename ParentT>
Vector3<T, ParentT> operator*(T left, const Vector3<T, ParentT>& right) {
    return Vector3<T, ParentT>(right.x * left, right.y * left, right.z * left);
}

template <typename T, template<typename> typename ParentT>
Vector3<T, ParentT> operator/(const Vector3<T, ParentT>& left, T right) {
    return Vector3<T, ParentT>(left.x / right, left.y / right, left.z / right);
}

template <typename T, template<typename> typename ParentT>
bool operator==(const Vector3<T, ParentT>& left, const Vector3<T, ParentT>& right) {
    return left.x == right.x && left.y == right.y && left.z == right.z;
}

template <typename T, template<typename> typename ParentT>
bool operator!=(const Vector3<T, ParentT>& left, const Vector3<T, ParentT>& right) {
    return !(left == right);
}

template <typename T, template<typename> typename ParentT>
T Mephi::Vector3<T, ParentT>::Len2() const {
    return (*this) ^ (*this);
}

template <typename T, template<typename> typename ParentT>
T Mephi::Vector3<T, ParentT>::Len() const {
    return std::sqrt(this->Len2());
}

template <typename T, template<typename> typename ParentT>
Vector3<T, ParentT> Mephi::Vector3<T, ParentT>::Mirror(Mephi::Axis axis) const {
    return {
        axis == Mephi::Axis::X ? -this->x : this->x,
        axis == Mephi::Axis::Y ? -this->y : this->y,
        axis == Mephi::Axis::Z ? -this->z : this->z
    };
}

template <typename T, template<typename> typename ParentT>
Vector3<T, ParentT> Mephi::Vector3<T, ParentT>::Clump(const Mephi::Vector3<T, ParentT>& minVec, 
                                                      const Mephi::Vector3<T, ParentT>& maxVec) const {
    return {
        std::max(std::min(this->x, maxVec.x), minVec.x),
        std::max(std::min(this->y, maxVec.y), minVec.y),
        std::max(std::min(this->z, maxVec.z), minVec.z)
    };
}

Common::Error TransformVector(Mephi::Vector3f& Vector, const Mephi::Transform Transform, 
                              Mephi::Axis Axis, const float AngleRadians = 0.01);

}

#endif /*MEPHI_REACTOR_SOURCE_VECTOR_INCLUDE_VECTOR_VECTOR_HPP*/