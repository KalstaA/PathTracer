#pragma once

typedef Eigen::Vector3d Vector;
typedef Eigen::Vector3d Point;
typedef Eigen::Vector3d Color;

struct Material
{
    Color color;
    float emission_strength;
    Color emission_color;
    float smoothness;
};
    
struct Camera
{
    Point position;
    Vector direction;
    Vector up;
    Vector right;
    float fov;
    float focus_distance;
};

struct Ray
{
    Point origin;
    Vector direction;
};

struct Hit
{
    bool did_hit;
    Material material;
    Vector normal;
    Point point;
    float distance;
};