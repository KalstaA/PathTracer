#pragma once

#include <random>
#include "types.hpp"

/**
 * @brief An object for creating random numbers and directions
 * 
 */
class RandomGenerator
{
private:

    std::random_device randomDevice; /* Random numbers provided by the OS */
    std::mt19937 randomInt; /* Uniformly distributed random integers */
    std::uniform_real_distribution<float> randZeroToOne; /* Uniformly distributed reals between 0 and 1 */
    std::normal_distribution<float> normal;  /* Random numbers satisfying the standard normal distribution */

public:
    RandomGenerator() : randomInt(randomDevice()), randZeroToOne(0, 1), normal(0, 1) {}
    ~RandomGenerator() = default;

    /**
     * @brief Creates a random direction, i.e., a random point on the unit sphere.
     * 
     * @return 3-dimensional vector pointing to a random direction
     */
    Vector randomDirection() {
        return Vector(normal(randomInt), normal(randomInt), normal(randomInt)).normalized();
    }

    /**
     * @brief Creates a random point inside the unit disk.
     * 
     * @return 2-dimensional vector inside of the unit disk.
     */
    Vector2 randomInCircle() {
        float angle = randZeroToOne(randomInt) * 2 * M_PI;
        float distance = randZeroToOne(randomInt);

        return Vector2(cos(angle), sin(angle)) * sqrt(distance);
    }
};