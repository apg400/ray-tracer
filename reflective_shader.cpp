#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{ 
    vec3 color = (1 - reflectivity) 
                 * shader->Shade_Surface(ray, intersection_point, normal, 0);
    if (recursion_depth > 1) {
        vec3 r = ray.direction - 2.0 * dot(normal, ray.direction) * normal;
        Ray reflected_ray(intersection_point, r.normalized());
        color = color + reflectivity * world.Cast_Ray(reflected_ray, recursion_depth-1);
    }
    return color;
}
