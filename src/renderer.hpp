#pragma once

#include <vector>

class Renderer
{
private:
    Scene scene_;
    Camera camera_;

    int resolution_x;
    int resolution_y;

    float anti_alias_radius;
    float depth_of_field;

    int max_bounces;
    int rays_per_pixel;

    float view_width;
    float view_height;

    Vector pixel_x;
    Vector pixel_y;

    Ray createRay(int x, int y) {
        
        Point origin = camera_.position;

        Vector topleft_pixel = camera_.direction - view_width * camera_.right + view_height * camera_.up;

        Vector target = topleft_pixel + pixel_y * y + pixel_x * x;
        Vector direction = (target - origin).normalized();

        return Ray(origin, direction);
    }
    
public:
    Renderer() {
        camera_ = scene_.getCamera();
        view_width = tan(1);
    }
    ~Renderer();

    std::vector<std::vector<Color>> render() {
        
    }
};
