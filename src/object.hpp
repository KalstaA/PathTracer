#pragma once

#include "types.hpp"

/**
 * @brief An abstract base class for any type of visible object in a scene
 * 
 */
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

    /**
     * @brief Calculate whether a given ray collides with the object.
     * 
     * If the ray collides with the object and the collision is closer than the current smallest distance,
     * the "rayHit" data structure will be updated according to the collision.
     * 
     * @param ray ray whose collision will be checked
     * @param rayHit address of a Hit data structure
     * @param smallestDistance 
     */
    virtual void collision(Ray& ray, Hit &rayHit, float& smallestDistance) = 0;
};