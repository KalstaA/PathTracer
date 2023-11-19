#pragma once

#include "types.hpp"

/**
 * @brief An object for representing a scene environment.
 * 
 * Currently only solid color or sky environments are supported.
 * 
 */
class Environment
{
private:
    Color skyColor_;
    Color horizonColor_;
    Color groundColor_;
public:

    /**
     * @brief Construct a new Environment with black void
     * 
     */
    Environment() : skyColor_(Color(0, 0, 0)), horizonColor_(Color(0, 0, 0)), groundColor_(Color(0, 0, 0)) {}

    /**
     * @brief Set a sky to the environment.
     * 
     * Parameters are optional, otherwise a default sky is created.
     * 
     * @param skyColor 
     * @param horizonColor 
     * @param groundColor 
     */
    void setSky(Color skyColor = Color(0.2, 0.5, 1.0), Color horizonColor = Color(0.7, 0.8, 0.8), Color groundColor = Color(0.1, 0.1, 0.1)) {
        skyColor_ = skyColor;
        horizonColor_ = horizonColor;
        groundColor_ = groundColor;
    }

    /**
     * @brief Calculate environment light collected by a runaway ray.
     * 
     * The ray direction determines what the ray should "see" in the environment.
     * 
     * @param ray a ray whose path doesn't intersect with any objects
     * @return Light collected from the environment by the ray
     */
    Light getLight(Ray& ray) {

        if (ray.direction(2) >= 0)
        {
            return horizonColor_ + (skyColor_ - horizonColor_) * pow(abs(ray.direction(2)), 0.8);
        }
        else
        {
            return horizonColor_ + (groundColor_ - horizonColor_) * pow(abs(ray.direction(2)), 0.4);
        }
    }

    // Getters
    Color getHorizonColor() { return horizonColor_; }
    Color getGroundColor() { return groundColor_; }
    Color getSkyColor() { return skyColor_; }
};
