#pragma once

#include "types.hpp"
#include <vector>

/**
 * @brief Implements the ray tracing algorithm.
 * 
 */
class Renderer
{
private:

    Scene scene_;
    Camera camera_;

    int resolution_x;
    int resolution_y;

    float anti_alias_radius = 1;
    float depth_of_field = 0;
    float focusDistance = 5;

    int max_bounces = 3;
    int rays_per_pixel = 5;

    float view_width;
    float view_height;

    Vector topleft_pixel;
    Vector pixel_x;
    Vector pixel_y;

    /**
     * @brief Create a Ray structure pointing to a certain pixel from the camera.
     * 
     * The ray target will be slighty randomized for anti-aliasing.
     * The ray origin will be randomized for depth of field.
     * 
     * @param x x-coordinate of the pixel
     * @param y y-coordinate of the pixel
     * @return Ray originating from the camera pointing to the pixel
     */
    Ray createRay(int x, int y) {

        // Depth of field effect randomizes the origin
        Vector2 jiggle = randomInCircle() * depth_of_field;
        Vector randomShift = jiggle(0) * pixel_x + jiggle(1) * pixel_y;
        Point origin = camera_.position + randomShift;

        // Anti-aliasing randomizes the target
        jiggle = randomInCircle() * anti_alias_radius;
        randomShift = jiggle(0) * pixel_x + jiggle(1) * pixel_y;
        Vector target = topleft_pixel + pixel_y * y + pixel_x * x + randomShift;

        Vector direction = (target - origin).normalized();

        return Ray{.origin = origin, .direction = direction};
    }

    /**
     * @brief Creates a Hit data structure representing a ray collision.
     * 
     * Checks whether the ray intersects any visible objects in the scene and picks the one closest to the camera.
     * 
     * @param ray a ray to be checked for collisions
     * @return Hit representing a possible collision
     */
    Hit rayCollision(Ray& ray) {

        float closestHit = INFINITY;
        Hit rayHit = { .did_hit = false };

        for (auto object : scene_.getObjects()) {
            object->collision(ray, rayHit, closestHit);
        }

        return rayHit;
    }

    /**
     * @brief Get all the light collected by a ray along its path.
     * 
     * @param ray ray to be traced
     * @return Light collected by the ray
     */
    Light trace(Ray& ray) {
        for (int bounce = 0; bounce < max_bounces; ++bounce)
        {
            Hit hit = rayCollision(ray);

            if (hit.did_hit && hit.distance > 0.001) {
                ray.origin = hit.point;

                Vector diffuse_direction = (randomDirection() + hit.normal).normalized();
                Vector specular_direction = reflect(ray.direction, hit.normal);

                ray.direction = diffuse_direction + hit.material.specularity * (specular_direction - diffuse_direction);
                Light emitted_light = hit.material.emission_strength * hit.material.emission_color;
                
                ray.light += emitted_light.cwiseProduct(ray.color);
                ray.color = ray.color.cwiseProduct(hit.material.color);
            }

            else 
            {
                ray.light += environmentLight(ray).cwiseProduct(ray.color);
                break;
            }
        }
        return ray.light;
    }
    
public:

    /**
     * @brief Construct a new Renderer object and initialize all necessary values.
     * 
     * @param res_x horizontal resolution of the rendering area
     * @param res_y vertical resolution of the rendering area
     * @param sceneToRender Scene object to be renderer
     */
    Renderer(int res_x, int res_y, Scene sceneToRender) {
        resolution_x = res_x;
        resolution_y = res_y;
        scene_ = sceneToRender;
        camera_ = scene_.getCamera();
        view_width = focusDistance * tan(camera_.fov / 2);
        view_height = view_width * (resolution_y - 1) / (resolution_x - 1);
        pixel_x = 2 * view_width / (resolution_x - 1) * camera_.direction.cross(camera_.up);
        pixel_y = - 2 * view_height / (resolution_y - 1) * camera_.up;
        topleft_pixel = camera_.direction * focusDistance - view_width * camera_.direction.cross(camera_.up) + view_height * camera_.up;
    }

    ~Renderer() = default;

    // TO-DO copy constructor and copy assigment

    /**
    * Get the result of the rendering. Takes no input parameters.
    *
    * @return a vector of vectors containing the colors of each pixel in the output image
    */
    auto render() {
        
        std::vector<std::vector<Color>> result(resolution_x, std::vector<Color> (resolution_y));
        
        for (int x = 0; x < resolution_x; x++)
        {
            for (int y = 0; y < resolution_y; y++)
            {
                Light totalLight(0, 0, 0);

                for (int rayNum = 0; rayNum < rays_per_pixel; rayNum++)
                {
                    Ray ray = createRay(x, y);
                    totalLight += trace(ray);
                }

                // Clamp for safety, sqrt for gamma correction
                result[x][y] = clamp(totalLight / rays_per_pixel).cwiseSqrt();
            }   
        }

        std::cout << "Rendering complete." << std::endl;

        return result;
    }
};
