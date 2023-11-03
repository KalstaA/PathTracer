#pragma once

class Ball : public Object
{
private:
    float radius_;

public:
    Ball(Vector position, float radius, Material material) : Object(position, material), radius_(radius) {}
    ~Ball() = default;

    // TO-DO copy constructor and copy assigment

    Hit collision(Ray ray) {

        Hit rayHit;
        rayHit.did_hit = false;

        Vector toBall = ray.origin - this->getPosition();

        float a = ray.direction.dot(ray.direction);
        float b = 2 * ray.direction.dot(toBall);
        float c = toBall.dot(toBall) - radius_ * radius_;

        float discriminant = b*b - 4*a*c;
        float distance = INFINITY;

        if (discriminant >= 0)
        {
            distance = (-b - sqrt(discriminant)) / (2*a);

            if (distance > 0)
            {
                rayHit.distance = distance;
                rayHit.material = this->getMaterial();
                rayHit.did_hit = true;
                rayHit.point = ray.origin + ray.direction * distance;
                rayHit.normal = (rayHit.point - this->getPosition()).normalized();
            }
        }

        return rayHit;
    }

    float getRadius() const {
        return radius_;
    }
};
