#pragma once

#include "object.hpp"

class Ball : public Object
{
private:
    float radius_;

public:
    Ball(Vector position, float radius, Material material) : Object(position, material), radius_(radius) {}

    // Default destructor, copy constructor and copy assignment
    ~Ball() = default;
    Ball& operator=(const Ball& that) = default;
    Ball(const Ball& that) = default;

    void collision(Ray ray, Hit &rayHit, float& smallestDistance) {

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

        return; //rayHit;
    }

    float getRadius() const { return radius_; }
};

std::ostream &operator<<(std::ostream& out, const Ball& ball) {
    out << "Ball at: (" << ball.getPosition().transpose() << ") with radius: " << ball.getRadius() << std::endl;
    return out;
}
