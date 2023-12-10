#pragma once

#include <Eigen/Dense>
#include <string>
#include <memory>

typedef Eigen::Vector3d Vector;
typedef Eigen::Vector3d Point;
typedef Eigen::Vector3d Color;
typedef Eigen::Vector3d Light;
typedef Eigen::Vector2d Vector2;
typedef Eigen::Matrix<double, 3, 3> Matrix;

// Forward declaration for Material class, such that the Hit struct knows the existence
class Material;
    
struct Camera
{
    Point position;
    Vector lookingAt;
    Vector direction;
    Vector up;
    Vector left;
    float fov;
    float focus_distance;
    float DoF;
};

struct Ray
{
    Point origin;
    Vector direction;
    bool inside_material = false;
    Color color = Color(1.0, 1.0, 1.0);
    Light light = Color(0.0, 0.0, 0.0);
};

struct Hit
{
    bool did_hit = false;
    std::shared_ptr<Material> material; // Has to be pointer, since compiler do not yet know anything about Material class
    Vector normal;
    Point point;
    float distance;
};
