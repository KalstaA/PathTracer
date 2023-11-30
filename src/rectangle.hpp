#pragma once

#include "object.hpp"
#include <vector>
#include <memory>

/**
 * @brief Representation of a box (rectangular cuboid) object in the scene.
 * 
 * The position represent the geometric center of the box.
 * 
 */
class Rectangle : public Object
{
private:
    float width_; /* Associated with y direction */
    float height_; /* Associated with z direction */

    std::vector<Vector> corners_;
    /*
    *
    *   0-------3
    *   |       |
    *   |       |
    *   |       | 
    *   1-------2
    * 
    * When looking towards positive x-direction
    */

public:
    Rectangle(Vector position, float width, float height, std::shared_ptr<Material> material) 
            : Object(position, material), width_(width), height_(height) {

        corners_.push_back(Vector(0, width_/2, height_/2) + position);
        corners_.push_back(Vector(0, width_/2, -height_/2) + position);
        corners_.push_back(Vector(0, -width_/2, -height_/2) + position);
        corners_.push_back(Vector(0, -width_/2, height_/2) + position);
    }

    /**
     * @brief Calculate whether a given ray collides with the rectangle.
     * 
     * If the ray collides with the rectangle and the collision is closer than the current smallest distance,
     * the "rayHit" data structure will be updated according to the collision.
     * 
     * @param ray ray whose collision will be checked
     * @param rayHit address of a Hit data structure
     * @param smallestDistance 
     */
    void collision(Ray& ray, Hit &rayHit, float& smallestDistance) {

        Vector topLeft = corners_[0];
        Vector bottomLeft = corners_[1];
        Vector bottomRight = corners_[2];

        Vector s1 = -bottomLeft + bottomRight;
        Vector s2 = -bottomLeft + topLeft;

        Vector normal = s1.cross(s2).normalized();

        float distance = (bottomLeft - ray.origin).dot(normal) / ray.direction.dot(normal);
        Vector intersection = -bottomLeft + ray.origin + ray.direction * distance;

        if (intersection.dot(s1) <= s1.squaredNorm() 
            && intersection.dot(s1) >= 0 
            && intersection.dot(s2) <= s2.squaredNorm() 
            && intersection.dot(s2) >= 0
        ) 
        {
            if (distance > 0 && distance < smallestDistance)
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
     * @brief Rotates the box around a given axis.
     * 
     * The center of rotation is the box's center of mass.
     * 
     * @param angle rotation in radians
     * @param axis axis of rotation (has to be normalized)
     */
    void rotate(float angle, Vector axis) {
        Eigen::AngleAxisd rotation(angle, axis);

        for (auto& corner : corners_) {
            corner = (rotation * (corner - this->getPosition())) + this->getPosition();
        }
    }

    /**
     * @brief Print box info to the desired output stream.
     * 
     * @param out output stream
     * @return std::ostream& the output stream
     */
    void printInfo(std::ostream& out) const {
        out << "Rectangle at: (" << this->getPosition().transpose() << ") with dimensions: " << width_ << "x" << height_ << ", with material: " << getMaterial()->getName() << std::endl;
    }

};
