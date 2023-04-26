#include <geometry/intersects.hpp>

#include <iostream>

#include <geometry/operations.hpp>
namespace geometry {

double DistBtw(const Line3f& line, const Point3f& pt) {
    assert(line.Dir().Len() > kEPS);
    return ((pt - line.Point()) ^ line.Dir()).Len() / (line.Dir().Len());
}

bool Intersect1(const Sphere3f& sph, const Line3f& line, Point3f* pt) {
    double dist = DistBtw(line, sph.Center());

    if (dist > sph.R()) {
        return false;
    }
    if (pt == nullptr) {
        return true;
    }

    Vector3f v = Normalize(line.Dir());
    Vector3f u = sph.Center() - line.Point();

    v *= v * u;

    Vector3f h = Normalize(v);
    h *= -std::sqrt(fabs(dist * dist - sph.R() * sph.R()));
    v += h;

    *pt = line.Point();
    *pt += v;
    if (!sph.Contains(*pt)) {
        std::cerr << pt->x << ' ' << pt->y << ' ' << pt->z << '\n';
    }
    assert(sph.Contains(*pt));
    return true;
}

bool Intersect1(const Sphere3f& sph, const Ray3f& ray, Point3f* pt) {
    double dist = DistBtw(ray, sph.Center());

    if (dist >= sph.R() - 2 * kEPS) {
        return false;
    }

    Vector3f v = Normalize(ray.Dir());
    Vector3f u = sph.Center() - ray.Point();

    bool is_inside = sph.ContainsB(ray.Start());

    if (v * u <= 0 && !is_inside) {
        return false;
    }

    if (pt == nullptr) {
        return true;
    }

    v *= v * u;

    Vector3f h = Normalize(ray.Dir());
    h *= std::sqrt(fabs(dist * dist - sph.R() * sph.R()));

    if (!is_inside) {
        h *= -1;
    }

    v += h;

    if (!is_inside && v * h > 0) {
        // throw an error
        // FIX:
    }
    assert(is_inside || v * h <= 0);

    *pt = ray.Point();
    *pt += v;
    if (!sph.ContainsB(*pt)) {
        // std::cout << sph.center() << ' ' << sph.r() << ' ' << *pt << ' ' << ((*pt - sph.center()).len() - sph.r()) <<
        // mlg::endl;
        // throw an error
        // FIX:
    }
    assert(sph.ContainsB(*pt));
    return true;
}

bool Intersect1(const Line3f& line, const Sphere3f& sph, Point3f* pt) {
    return Intersect1(sph, line, pt);
}

bool Intersect1(const Ray3f& ray, const Sphere3f& sph, Point3f* pt) {
    return Intersect1(sph, ray, pt);
}

Vector3f GetReflection(const Vector3f& v, const Vector3f& u) {
    Vector3f res = Normalize(u);
    res *= 2 * (res * v);
    res -= v;
    return res;
}

bool Intersect1(const Line3f& line, const Plane3f& pln, Point3f* pt) {
    if (pln || line.Dir()) {
        return false;
    }
    if (pt == nullptr) {
        return true;
    }

    Vector3f v = line.Dir();
    v *= -pln.Dist(line.Point()) / (pln.Normal() * line.Dir());

    *pt = line.Point();
    *pt += v;

    return true;
}

bool Intersect1(const Ray3f& ray, const Plane3f& pln, Point3f* pt) {
    if (pln || ray.Dir()) {
        return false;
    }

    Vector3f v = ray.Dir();
    v *= pln.Dist(ray.Start()) / (pln.Normal() * ray.Dir());

    if (v * ray.Dir() < 0) {
        return false;
    }
    if (pt == nullptr) {
        return true;
    }

    *pt = ray.Start();
    *pt += v;

    // assert(pln.Contains(*pt));

    return true;
}

bool Intersect1(const Plane3f& pln, const Ray3f& ray, Point3f* pt) {
    return Intersect1(ray, pln, pt);
}

bool Intersect1(const Plane3f& pln, const Line3f& line, Point3f* pt) {
    return Intersect1(line, pln, pt);
}

}  // namespace geometry
