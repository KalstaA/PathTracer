#pragma once

class Ball
{
private:
    Vector position_;
    float radius_;
    Material material_;

public:
    Ball(Vector position, float radius, Material material) : position_(position), radius_(radius), material_(material) {}
    ~Ball() = default;
    
    // TO-DO copy constructor and copy assigment

    Point getPosition() const {
        return position_;
    }

    float getRadius() const {
        return radius_;
    }

    Material getMaterial() const {
        return material_;
    }
};
