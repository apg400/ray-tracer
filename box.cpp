#include <limits>
#include "box.h"

// Return whether the ray intersects this box.
bool Box::Intersection(const Ray& ray) const
{
    vec3 t_min(0,0,0), t_max(0,0,0);

    for (int i = 0; i < 3; i++) {
        double ep = ray.endpoint[i];
        double d = ray.direction[i];
        if (d > 0) {
            t_min[i] = (lo[i] - ep) / d;
            t_max[i] = (hi[i] - ep) / d;
        } else if (d < 0) {
            t_min[i] = (hi[i] - ep) / d;
            t_max[i] = (lo[i] - ep) / d;
        } else {
            t_min[i] = (lo[i] >= ep) ? std::numeric_limits<double>::max() 
                       : std::numeric_limits<double>::min();
            t_max[i] = (hi[i] <= ep) ? std::numeric_limits<double>::min()
                       : std::numeric_limits<double>::max();
        }
    } 

    double t_lower_bound = std::max(t_min[0], std::max(t_min[1], t_min[2]));
    double t_upper_bound = std::min(t_max[0], std::min(t_max[1], t_max[2]));

    return t_upper_bound > t_lower_bound; 
}

// Compute the smallest box that contains both *this and bb.
Box Box::Union(const Box& bb) const
{
    Box box;
    box.lo = componentwise_min(this->lo, bb.lo);
    box.hi = componentwise_max(this->hi, bb.hi);
    return box;
}

// Enlarge this box (if necessary) so that pt also lies inside it.
void Box::Include_Point(const vec3& pt)
{
    lo = componentwise_min(lo, pt);
    hi = componentwise_max(hi, pt);
}

// Create a box to which points can be correctly added using Include_Point.
void Box::Make_Empty()
{
    lo.fill(std::numeric_limits<double>::infinity());
    hi = -lo;
}
