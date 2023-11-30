#pragma once

#include "types.hpp"
#include "material.hpp"

#include <memory>

/**
 * @brief An abstract base class for any type of visible object in a scene
 * 
 */
class Object
{
private:
    Vector position_;
    std::shared_ptr<Material> material_;
    Vector up_;
    Vector forward_;

public:
    Object(Vector position, std::shared_ptr<Material> material, Vector up = Vector(0, 0, 1), Vector forward = Vector(1, 0, 0)) 
                    : position_(position), material_(material), up_(up), forward_(forward) {}
    
    virtual ~Object() { }

    Vector getPosition() const { return position_; }
    std::shared_ptr<Material> getMaterial() const { return material_; }
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