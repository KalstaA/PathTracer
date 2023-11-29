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

public:
    Object(Vector position, Material material) 
                    : position_(position), material_(material) {}
    
    virtual ~Object() { }

    Vector getPosition() const { return position_; }
    Material getMaterial() const { return material_; }

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

    /**
     * @brief Print object info to the desired output stream.
     * 
     * @param out output stream
     * @param object object to be printed
     * @return std::ostream& the output stream
     */
    virtual void printInfo(std::ostream& out) const = 0;
};

/**
 * @brief <<-operator overload for printing object info to the desired output stream.
 * 
 * printInfo-method for each of the objects is called.
 * 
 * @param out output stream
 * @param object object to be printed
 * @return std::ostream& the output stream
 */
std::ostream &operator<<(std::ostream& out, const Object& object) {
    object.printInfo(out);
    return out;
}