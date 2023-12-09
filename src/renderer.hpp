#pragma once

#include "types.hpp"
#include <vector>
#include "randomgenerator.hpp"
#include <iostream>
#include <omp.h>
#include <chrono>
#include <memory>

/**
 * @brief Implements the ray tracing algorithm.
 * 
 */
class Renderer
{
private:

    std::shared_ptr<Scene> scene_;
    Camera camera_;
    RandomGenerator rnd_;

    int resolution_x;
    int resolution_y;
    std::vector<std::vector<Color>> result;

    float anti_alias_radius = 1;

    int max_bounces = 5; // Default value if anyone do not change

    float view_width;
    float view_height;

    Vector topleft_pixel;
    Vector pixel_x;
    Vector pixel_y;

    int progressBarWidth = 100;

    /**
     * @brief Clamps the given color values, i.e., ensures that the maximum value for each R, G, B is 1.
     * 
     * @param input the color vector to be clamped
     * @return Color with the values clamped
     */
    Color clamp(Color input) {
        float R = input(0) > 1 ? 1 : input(0);
        float G = input(1) > 1 ? 1 : input(1);
        float B = input(2) > 1 ? 1 : input(2);
        return Color(R, G, B);
    }

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
        Vector2 jiggle = rnd_.randomInCircle() * camera_.DoF;
        Vector randomShift = jiggle(0) * pixel_x + jiggle(1) * pixel_y;
        Point origin = camera_.position + randomShift;

        // Anti-aliasing randomizes the target
        jiggle = rnd_.randomInCircle() * anti_alias_radius;
        randomShift = jiggle(0) * pixel_x + jiggle(1) * pixel_y;
        Vector target = topleft_pixel + pixel_y * y + pixel_x * x + randomShift;

        Vector direction = (target-origin).normalized();

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

        for (auto object : (*scene_).getObjects()) {
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

            if (hit.did_hit && hit.distance > 0.0001) {
                // Update ray according to material properties
                (*hit.material).updateRay(ray, hit);
            }
            else 
            {
                ray.light += (*scene_).getEnvironment().getLight(ray).cwiseProduct(ray.color);
                break;
            }
        }
        return ray.light;
    }

    /**
     * @brief Prints really cool progress bar indicating the progress of the rendering process
     * 
     * @param sample Integer representing the number of the sample
     * @param samples Total number of the samples
     */
    void progressBar(int sample, int samples) {
        std::cout << "[";
        int pos = progressBarWidth * (sample + 1)/samples;
        for (int i = 0; i < progressBarWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << std::round((sample + 1) * 100.0 / samples) << " %\r";
        std::cout.flush();
    }
    
public:

    /**
     * @brief Construct a new Renderer object and initialize all necessary values.
     * 
     * @param res_x horizontal resolution of the rendering area
     * @param res_y vertical resolution of the rendering area
     * @param sceneToRender Scene object to be renderer
     */
    Renderer(int res_x, int res_y, std::shared_ptr<Scene> sceneToRender) {
        resolution_x = res_x;
        resolution_y = res_y;
        result = std::vector<std::vector<Color>>(resolution_x, std::vector<Color> (resolution_y));
        scene_ = sceneToRender;
        camera_ = (*scene_).getCamera();
        view_width = camera_.focus_distance * tan(camera_.fov / 2);
        view_height = view_width * (resolution_y - 1) / (resolution_x - 1);
        pixel_x = -2 * view_width / (resolution_x - 1) * camera_.left;
        pixel_y = - 2 * view_height / (resolution_y - 1) * camera_.up;
        topleft_pixel = camera_.position + camera_.focus_distance * camera_.direction + view_width * camera_.left + view_height * camera_.up;
    }

    /**
     * @brief Function to set maximum bounces for rays
     * 
     * @param bounces Number of maximum bounces ray can take
     */
    void setMaxBounces(int bounces) {
        max_bounces = bounces;
    }

    /**
     * @brief Rendering function that uses all available CPU cores
     * 
     * @param samples Amount of samples that will be taken for each pixel
     */
    auto parallelRender(int samples) {

        auto startTime = std::chrono::high_resolution_clock::now();        
        std::vector<std::vector<Color>> result(resolution_x, std::vector<Color> (resolution_y));

        std::cout << "Rendering started..." << std::endl;

        for (int sample = 0; sample < samples; ++sample)
        {
            float weight = 1.0 / (sample + 1);

            #pragma omp parallel for num_threads(omp_get_max_threads())
            for (int x = 0; x < resolution_x; ++x)
            {
                for (int y = 0; y < resolution_y; ++y)
                {
                    Ray ray = createRay(x, y);
                    Light totalLight = trace(ray);
                    result[x][y] = clamp(result[x][y] * (1 - weight) + weight * totalLight.cwiseSqrt());
                }
            }
            progressBar(sample, samples);
        }

        std::cout << std::endl;
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = endTime - startTime; 
        std::cout << "Used " << omp_get_max_threads() << " threads.\n" << std::endl;
        std::cout << "Rendering completed in " << duration.count() << " seconds.\n" << std::endl;
        
        return result;
    }

    /**
     * @brief Set the depth of field for the camera in the renderer.
     * 
     * @param dof Value for depth of field
     */
    void setDof(float dof) {
        camera_.DoF = dof;
    }
};
