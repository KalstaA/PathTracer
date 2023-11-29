#pragma once

#include "object.hpp"
#include <vector>

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

    std::vector<Vector> corners_;
    /*
    *       4.......7
    *     / :     / :
    *   0-------3   :
    *   |   :   |   :
    *   |   5...|...6
    *   | /     | /
    *   1-------2
    * 
    * When looking towards positive x-direction
    */

   std::list<std::vector<int>> sides_ = {   {0, 1, 2},
                                            {3, 2, 6},
                                            {7, 6, 5},
                                            {4, 5, 1},
                                            {4, 0, 3},
                                            {1, 5, 6}};

public:
    Box(Vector position, float width, float height, float depth, Material material) 
            : Object(position, material), width_(width), height_(height), depth_(depth) {

        corners_.push_back(Vector(-depth_/2, width_/2, height_/2) + position);
        corners_.push_back(Vector(-depth_/2, width_/2, -height_/2) + position);
        corners_.push_back(Vector(-depth_/2, -width_/2, -height_/2) + position);
        corners_.push_back(Vector(-depth_/2, -width_/2, height_/2) + position);

        corners_.push_back(Vector(depth_/2, width_/2, height_/2) + position);
        corners_.push_back(Vector(depth_/2, width_/2, -height_/2) + position);
        corners_.push_back(Vector(depth_/2, -width_/2, -height_/2) + position);
        corners_.push_back(Vector(depth_/2, -width_/2, height_/2) + position);
    }

    /**
     * @brief Calculate whether a given ray collides with the box.
     * 
     * If the ray collides with the box and the collision is closer than the current smallest distance,
     * the "rayHit" data structure will be updated according to the collision.
     * 
     * @param ray ray whose collision will be checked
     * @param rayHit address of a Hit data structure
     * @param smallestDistance 
     */
    void collision(Ray& ray, Hit &rayHit, float& smallestDistance) {

        // Check first whether ray collides with the bounding ball
        Vector toBall = ray.origin - this->getPosition();
        float radius = width_ * width_ + height_ * height_ + depth_ * depth_;
        float a = ray.direction.dot(ray.direction);
        float b = 2 * ray.direction.dot(toBall);
        float c = toBall.dot(toBall) - radius;
        float discriminant = b*b - 4*a*c;
        if (discriminant < 0) return;

        // If inside ball, check sides
        for (auto side : sides_) {

            Vector topLeft = corners_[side[0]];
            Vector bottomLeft = corners_[side[1]];
            Vector bottomRight = corners_[side[2]];

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
    }

    float getWidth() const { return width_; }
    float getHeight() const { return height_; }
    float getDepth() const { return depth_; }

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
        out << "Box at: (" << this->getPosition().transpose() << ") with dimensions: " << width_ << "x" << height_ << "x" << depth_ << ", with material: " << this->getMaterial().name << std::endl;
    }

};
