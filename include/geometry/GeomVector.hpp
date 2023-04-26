#ifndef GEOMETRY_VECTOR_HPP
#define GEOMETRY_VECTOR_HPP
#include <geometry/Float.hpp>
#include <cassert>
#include <cmath>

namespace geometry {

/**
 * @brief The Vector2 class
 */
template <typename NumT>
class Vector2 {

public:
    NumT x, y;

    Vector2(NumT x_new, NumT y_new) : x(x_new), y(y_new) {
    }
    Vector2() : Vector2(0, 0) {
    }

    template <typename T>
    explicit Vector2(const Vector2<T>& oth) : x(oth.x), y(oth.y) {
    }

    double Len() const {
        return sqrt(x * x + y * y);
    }
    double Len2() const {
        return x * x + y * y;
    }

    Vector2<NumT>& operator+=(const Vector2<NumT>& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector2<NumT>& operator-=(const Vector2<NumT>& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vector2<NumT>& operator*=(NumT rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    Vector2<NumT>& operator/=(NumT rhs) {
        x /= rhs;
        y /= rhs;
        return *this;
    }
};

/**
 * @brief getOrthogonal - create vector orthogonal to given
 * @param v - base vector
 * @return orthogonal to v
 */
template <typename NumT>
Vector2<NumT> GetOrthogonal(const Vector2<NumT>& v) {
    return Vector2<NumT>{-v.y, v.x};
}

/**
 * @brief The Vector3 class
 */
template <typename NumT>
class Vector3 {
public:
    NumT x, y, z;

    Vector3(NumT x_new, NumT y_new, NumT z_new) : x(x_new), y(y_new), z(z_new) {
    }

    Vector3() : Vector3(0, 0, 0) {
    }

    explicit Vector3(const Vector2<NumT>& v) : Vector3(v.x, v.y, 0) {
    }

    template <typename T>
    explicit Vector3(const Vector3<T>& oth) : x(oth.x), y(oth.y), z(oth.z) {
    }

    double Len() const {
        return sqrt(x * x + y * y + z * z);
    }
    double Len2() const {
        return (x * x + y * y + z * z);
    }

    Vector3<NumT>& operator+=(const Vector3<NumT>& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vector3<NumT>& operator-=(const Vector3<NumT>& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vector3<NumT>& operator*=(NumT rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    Vector3<NumT>& operator/=(NumT rhs) {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }
};

/**
 * @brief dotProd - scalar multiplication of two vectors.
 * @param v1 - first vector
 * @param v2 - second vector
 * @return (v1, v2)
 */
template <typename NumT>
NumT DotProd(const Vector2<NumT>& v1, const Vector2<NumT>& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

/**
 * @brief dotProd - scalar multiplication of two vectors.
 * @param v1 - first vector
 * @param v2 - second vector
 * @return (v1, v2)
 */
template <typename NumT>
NumT DotProd(const Vector3<NumT>& v1, const Vector3<NumT>& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/**
 * @brief operator * alias to dotprod
 */
template <typename NumT>
NumT operator*(const Vector2<NumT>& v1, const Vector2<NumT>& v2) {
    return DotProd(v1, v2);
}

/**
 * @brief operator * alias to dotprod
 * @ref dotProd
 */
template <typename NumT>
NumT operator*(const Vector3<NumT>& v1, const Vector3<NumT>& v2) {
    return DotProd(v1, v2);
}

/**
 * @brief crossProd return z value of vector multiplication
 * It's equal to |v1| * |v2| * sin angle(v1, v2).
 * @param v1 - first
 * @param v2 - second
 * @return z value of vector multiplication == |[v1, v2]|
 */
template <typename NumT>
NumT CrossProd(const Vector2<NumT>& v1, const Vector2<NumT>& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

/**
 * @brief crossProd - vector multiplication of two vectors.
 * @param v1 - lhs
 * @param v2 - rhs
 * @return [v1, v2]
 */
template <typename NumT>
Vector3<NumT> CrossProd(const Vector3<NumT>& v1, const Vector3<NumT>& v2) {
    return Vector3<NumT>{
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
    };
}

template <typename NumT>
NumT operator^(const Vector2<NumT>& v1, const Vector2<NumT>& v2) {
    return CrossProd(v1, v2);
}

template <typename NumT>
Vector3<NumT> operator^(const Vector3<NumT>& v1, const Vector3<NumT>& v2) {
    return CrossProd(v1, v2);
}

template <typename NumT>
bool IsParallel(const Vector2<NumT>& v1, const Vector2<NumT>& v2) {
    return IsZero(v1 ^ v2);
}

template <typename NumT>
bool IsParallel(const Vector3<NumT>& v1, const Vector3<NumT>& v2) {
    return IsZero((v1 ^ v2).Len());
}

template <typename NumT>
bool IsCollin(const Vector2<NumT>& v1, const Vector2<NumT>& v2) {
    return IsParallel(v1, v2) && (v1 * v2) >= 0;
}

template <typename NumT>
bool IsColling(const Vector3<NumT>& v1, const Vector3<NumT>& v2) {
    return IsParallel(v1, v2) && (v1 * v2) >= 0;
}

template <typename NumT>
bool operator||(const Vector2<NumT>& v1, const Vector2<NumT>& v2) {
    return IsParallel(v1, v2);
}

template <typename NumT>
bool operator||(const Vector3<NumT>& v1, const Vector3<NumT>& v2) {
    return IsParallel(v1, v2);
}

using Vector2f = Vector2<double>;
using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned>;

using Vector3f = Vector3<double>;
using Vector3i = Vector3<int>;
using Vector3u = Vector3<unsigned>;

Vector2f Normalize(Vector2f v);
Vector3f Normalize(Vector3f v);

Vector2f Rotate(const Vector2f& v, double a);
Vector3f RotateZ(const Vector3f& v, double a);

bool operator==(const Vector2f& lhs, const Vector2f& rhs);
bool operator==(const Vector3f& lhs, const Vector3f& rhs);

inline bool operator!=(const Vector2f& rhs, const Vector2f& lhs) {
    return !(lhs == rhs);
}
inline bool operator!=(const Vector3f& lhs, const Vector3f& rhs) {
    return !(lhs == rhs);
}

}  // namespace Geometry
#endif
