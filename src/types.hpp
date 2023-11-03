#pragma once

typedef Eigen::Vector3d Vector;
typedef Eigen::Vector3d Point;
typedef Eigen::Vector3d Color;
typedef Eigen::Vector3d Light;
typedef Eigen::Vector2d Vector2;

struct Material
{
    Color color;
    float emission_strength;
    Color emission_color;
    float specularity;
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
    Color color;
    Light light;
};

struct Hit
{
    bool did_hit;
    Material material;
    Vector normal;
    Point point;
    float distance;
};

Vector reflect(Vector in, Vector normal) {
    return in - 2 * in.dot(normal) * normal;
}

Vector randomDirection() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> d(0, 1); 

    return Vector(d(gen), d(gen), d(gen)).normalized();
}

float randZeroToOne() {
    return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}

Vector2 randomInCircle() {
    float angle = randZeroToOne() * 2 * M_PI;
    float distance = randZeroToOne();

    return Vector2(cos(angle), sin(angle)) * sqrt(distance);
}

Color clamp(Color input) {
    float R = input(0) > 1 ? 1 : input(0);
    float G = input(1) > 1 ? 1 : input(1);
    float B = input(2) > 1 ? 1 : input(2);
    return Color(R, G, B);
}