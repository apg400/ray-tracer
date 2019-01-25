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
        vec3 light_dir = light->position - intersection_point;
        Ray shadow_ray(intersection_point, light_dir);
        Hit interference = world.Closest_Intersection(shadow_ray);
        if (!interference.object) {
            vec3 h = (light_dir.normalized() + (-ray.direction).normalized()).normalized();
            color += color_diffuse * light->brightness * std::max(0.0, dot(normal,light_dir)) 
                     + color_specular * light->brightness 
                     * pow(dot(normal,h), specular_power); 
        }
    }
    return color;
}
