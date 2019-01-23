#include "sphere.h"
#include "ray.h"
#include <cmath>

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    // t = [-b +- sqrt(b^2 - 4ac)] / 2a
    vec3 w = ray.endpoint - center.normalized();
    double a = dot(ray.direction, ray.direction);
    double b = 2 * dot(w, ray.direction);
    double c = dot(w, w) - (radius * radius);
    double discriminant = (b * b) - (4 * a * c);
    if (discriminant > small_t) {
        double x = (b > 0) ? -0.5 * (b + sqrt(discriminant)) : -0.5 * (b - sqrt(discriminant));
        double t1 = x / a;
        double t2 = c / x;
        if (t1 > t2) std::swap(t1, t2);
        if (t1 > small_t) return {this, t1, 0};
        if (t2 > small_t) return {this, t2, 0};
    }
    return {nullptr, 0, 0};
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    // first get vector from center to the point, then normalize it
    return (point - center).normalized();
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
