#pragma once

#include <list>
#include "types.hpp"

class Scene
{
private:
    std::list<Ball> balls_;
    Camera camera_;

public:
    Scene() = default;
    Scene(Camera camera, std::list<Ball> balls) : camera_(camera), balls_(balls) {}
    ~Scene() = default;

    // Default copying for now - list is copied (are the balls copied as well?)
    Scene& operator=(const Scene& that) = default;
    Scene(const Scene& that) = default;

    Camera getCamera() const { return camera_; }

    std::list<Ball> getBalls() const { return balls_; }
};

std::ostream &operator<<(std::ostream& out, const Scene& scene) {
    out << "\n========== SCENE INFORMATION ==========" << std::endl;
    for (auto ball : scene.getBalls())
    {
        out << ball;
    }
    out << std::endl;
    auto cam = scene.getCamera();
    out << "Camera at: (" << cam.position.transpose() << ") looking at: (" << cam.direction.transpose() << ") with FOV: " << cam.fov / M_PI * 180 << " degrees" << std::endl;
    out << "=======================================\n" << std::endl;
    return out;
}
