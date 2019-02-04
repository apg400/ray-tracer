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
    // Sets the base color to the ambient color
    vec3 color = color_ambient * world.ambient_color * world.ambient_intensity;

    // Iterates through each light in the scene to determine its affect on the color
    // of the intersected object.
    for (Light* light : world.lights) {
        
        vec3 light_dir = (light->position - intersection_point).normalized();
        Ray shadow_ray(intersection_point, light_dir);

        // Checks whether there is something in between the intersection point 
        // and the light
        Hit interference = world.Closest_Intersection(shadow_ray);

        // Checks if the detected hit is behind the light. Hits behind the light are set 
        // to null and ignored, hits in front of the light are left as is
        if (interference.object != nullptr) {
            double light_dist = (shadow_ray.endpoint - light->position).magnitude();
            if (light_dist > small_t && light_dist <= interference.dist) 
                interference = {nullptr, 0, 0};
        }

        // If there is no object blocking the light then the color is calculated 
        // with the light's influence
        if (!interference.object || !world.enable_shadows) {
            vec3 d = (intersection_point - light->position).normalized();
            vec3 ideal_r = d - 2.0 * dot(normal, d) * normal;
            // Calculates color based on light, C = Ambient + Diffuse + Specular
            // C = C + k_d * L * max(n*l) + k_s * L * max(v*r)^a
            color = color + color_diffuse * std::max(0.0, dot(normal,light_dir)) 
                    * light->Emitted_Light(light->position - intersection_point) 
                    + color_specular 
                    * pow(std::max(0.0, dot(-ray.direction, ideal_r)), specular_power) 
                    * light->Emitted_Light(light->position - intersection_point);
        }
    }
    return color;
}
