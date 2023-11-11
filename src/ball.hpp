#pragma once

#include "object.hpp"

/**
 * @brief Representation of a mathematical ball object in the scene.
 * 
 */
class Ball : public Object
{
private:
    float radius_;

public:
    Ball(Vector position, float radius, Material material) : Object(position, material), radius_(radius) {}

    ~Ball() = default;
    Ball& operator=(const Ball& that) = default;
    Ball(const Ball& that) = default;


    /**
     * @brief Calculate whether a given ray collides with the ball.
     * 
     * If the ray collides with the ball and the collision is closer than the current smallest distance,
     * the "rayHit" data structure will be updated according to the collision.
     * 
     * @param ray ray whose collision will be checked
     * @param rayHit address of a Hit data structure
     * @param smallestDistance 
     */
    void collision(Ray& ray, Hit &rayHit, float& smallestDistance) {

        Vector toBall = ray.origin - this->getPosition();

        float a = ray.direction.dot(ray.direction);
        float b = 2 * ray.direction.dot(toBall);
        float c = toBall.dot(toBall) - radius_ * radius_;

        float discriminant = b*b - 4*a*c;

        if (discriminant >= 0)
        {
            float distance = (-b - sqrt(discriminant)) / (2*a);

            if (distance > 0 && distance < smallestDistance)
            {
                smallestDistance = distance;
                rayHit.distance = distance;
                rayHit.material = this->getMaterial();
                rayHit.did_hit = true;
                rayHit.point = ray.origin + ray.direction * distance;
                rayHit.normal = (rayHit.point - this->getPosition()).normalized();
            }
        }

        return;
    }

    float getRadius() const { return radius_; }
};

/**
 * @brief Print ball info to the desired output stream.
 * 
 * @param out output stream
 * @param ball ball to be printed
 * @return std::ostream& the output stream
 */
std::ostream &operator<<(std::ostream& out, const Ball& ball) {
    out << "Ball at: (" << ball.getPosition().transpose() << ") with radius: " << ball.getRadius() << ", with material: " << ball.getMaterial().name << std::endl;
    return out;
}
