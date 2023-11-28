#pragma once

#include <Eigen/Dense>
#include <string>

typedef Eigen::Vector3d Vector;
typedef Eigen::Vector3d Point;
typedef Eigen::Vector3d Color;
typedef Eigen::Vector3d Light;
typedef Eigen::Vector2d Vector2;
typedef Eigen::Matrix<double, 3, 3> Matrix;

struct Material
{
    Color color = Color(1.0, 1.0, 1.0);
    float emission_strength = 0.0;
    Color emission_color = Color(1.0, 1.0, 1.0);
    float specularity = 0.0;
    float clearcoat = 0.0;
    Color clearcoat_color = Color(1.0, 1.0, 1.0);
    std::string name = "[UNNAMED MATERIAL]";
};
    
struct Camera
{
    Point position;
    Vector direction;
    Vector up;
    float fov;
    float focus_distance;
};

struct Ray
{
    Point origin;
    Vector direction;
    Color color = Color(1.0, 1.0, 1.0);
    Light light = Color(0.0, 0.0, 0.0);
};

struct Hit
{
    bool did_hit = false;
    Material material;
    Vector normal;
    Point point;
    float distance;
};