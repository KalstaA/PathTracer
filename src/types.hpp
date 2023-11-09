#pragma once

#include <../libs/Eigen/Dense>

typedef Eigen::Vector3d Vector;
typedef Eigen::Vector3d Point;
typedef Eigen::Vector3d Color;
typedef Eigen::Vector3d Light;
typedef Eigen::Vector2d Vector2;

struct Material
{
    Color color = Color(1.0, 1.0, 1.0);
    float emission_strength = 0.0;
    Color emission_color = Color(1.0, 1.0, 1.0);
    float specularity = 0.0;
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
    bool did_hit;
    Material material;
    Vector normal;
    Point point;
    float distance;
};

Light environmentLight(Ray ray) {

    Color skyColor(0.2, 0.5, 1.0);
    Color horizonColor(0.7, 0.8, 0.8);
    Color groundColor(0.1, 0.1, 0.1);

    if (ray.direction(2) >= 0)
    {
        return horizonColor + (skyColor - horizonColor) * pow(abs(ray.direction(2)), 0.8);
    }
    else
    {
        return horizonColor + (groundColor - horizonColor) * pow(abs(ray.direction(2)), 0.4);
    }
}