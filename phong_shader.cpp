#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"
#include <cmath>

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color = color_ambient * world.ambient_intensity;
    for (Light* light : world.lights) {
        vec3 light_dir = (light->position - intersection_point).normalized();
        Ray shadow_ray(intersection_point, light_dir);
        Hit interference = world.Closest_Intersection(shadow_ray);
        if (!interference.object) {
            vec3 d = (intersection_point - light->position).normalized();
            vec3 r = d - 2.0 * dot(normal, d) * normal;
            color = color + color_diffuse * std::max(0.0, dot(normal,light_dir)) 
                    * light->Emitted_Light(light->position - intersection_point) 
                    + color_specular 
                    * pow(std::max(0.0,dot(-ray.direction, r)), specular_power) 
                    * light->Emitted_Light(light->position-intersection_point);
        }
    }
    return color;
}
