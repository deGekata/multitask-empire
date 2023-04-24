#ifndef GEOMETRY_INTERSECTS_HPP
#define GEOMETRY_INTERSECTS_HPP

#include <geometry/Point.hpp>
#include <geometry/Line.hpp>
#include <geometry/Sphere.hpp>
#include <geometry/Plane.hpp>

namespace Geometry {
double distBtw(const Line3f& line, const Point3f& pt);

Vector3f getReflection(const Vector3f& v, const Vector3f& u);

bool intersect1(const Sphere3f& sph, const Line3f& line, Point3f* pt = nullptr);
bool intersect1(const Sphere3f& sph, const Ray3f& ray, Point3f* pt = nullptr);

bool intersect1(const Line3f& line, const Sphere3f& sph, Point3f* pt = nullptr);
bool intersect1(const Ray3f& ray, const Sphere3f& sph, Point3f* pt = nullptr);

bool intersect1(const Ray3f& ray, const Plane3f& pln, Point3f* pt = nullptr);
bool intersect1(const Line3f& line, const Plane3f& pln, Point3f* pt = nullptr);

bool intersect1(const Plane3f& pln, const Ray3f& ray, Point3f* pt = nullptr);
bool intersect1(const Plane3f& pln, const Line3f& line, Point3f* pt = nullptr);

}  // namespace Geometry

#endif /* GEOMETRY_INTERSECTS_HPP */
