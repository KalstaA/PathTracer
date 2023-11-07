#pragma once

#include "types.hpp"
#include <string>

class Object
{
private:
    Vector position_;
    Material material_;
    Vector up_;
    Vector forward_;

public:
    Object(Vector position, Material material, Vector up = Vector(0, 0, 1), Vector forward = Vector(1, 0, 0)) 
                    : position_(position), material_(material), up_(up), forward_(forward) {}
    
    // Default destructor, copy constructor and copy assignment
    ~Object() = default;
    Object& operator=(const Object& that) = default;
    Object(const Object& that) = default;

    Vector getPosition() const { return position_; }
    Material getMaterial() const { return material_; }
    Vector getUp() const { return up_; }
    Vector getForward() const { return forward_; }

    virtual void collision(Ray ray, Hit &rayHit, float& smallestDistance) = 0;
};