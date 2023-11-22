#pragma once

#include "object.hpp"

/**
 * @brief Representation of a box (rectangular cuboid) object in the scene.
 * 
 * The position represent the geometric center of the box.
 * 
 */
class Box : public Object
{
private:
    float width_; /* Associated with y direction */
    float height_; /* Associated with z direction */
    float depth_; /* Associated with x direction */

public:
    Box(Vector position, float width, float height, float depth, Material material) : Object(position, material), width_(width), height_(height), depth_(depth) {}

    /**
     * @brief Calculate whether a given ray collides with the box.
     * 
     * If the ray collides with the ball and the collision is closer than the current smallest distance,
     * the "rayHit" data structure will be updated according to the collision.
     * 
     * @param ray ray whose collision will be checked
     * @param rayHit address of a Hit data structure
     * @param smallestDistance 
     */
    void collision(Ray& ray, Hit &rayHit, float& smallestDistance) {

        Vector bottomLeft = Vector(-depth_/2, width_/2, -height_/2) + this->getPosition();
        Vector topLeft = Vector(-depth_/2, width_/2, height_/2) + this->getPosition();
        Vector bottomRight = Vector(-depth_/2, -width_/2, -height_/2) + this->getPosition();

        Vector s1 = -bottomLeft + bottomRight;
        Vector s2 = -bottomLeft + topLeft

        Vector normal = s1.crossP(s2).normalized();

        float distance = (bottomLeft - ray.origin).dot(normal) / ray.direction.dot(normal);
        Vector intersection = -bottomLeft + ray.origin + ray.direction * distance;

        if (intersection.dot(s1) <= s1.squaredNorm() 
            && intersection.dot(s1) >= 0 
            && intersection.dot(s2) <= s2.squaredNorm() 
            && intersection.dot(s2) >= 0
        ) 
        {
            if (distance < smallestDistance)
            {
                smallestDistance = distance;
                rayHit.distance = distance;
                rayHit.material = this->getMaterial();
                rayHit.did_hit = true;
                rayHit.point = ray.origin + ray.direction * distance;
                rayHit.normal = normal;
            }
        }

    }

    float getWidth() const { return width_; }
    float getHeight() const { return height_; }

    /**
     * @brief Print box info to the desired output stream.
     * 
     * @param out output stream
     * @return std::ostream& the output stream
     */
    void printInfo(std::ostream& out) const {
        out << "Box at: (" << this->getPosition().transpose() << ") with dimensions: " << width_ << "x" << height_ << ", with material: " << this->getMaterial().name << std::endl;
    }

};
