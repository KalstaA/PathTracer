#pragma once

#include <list>
#include "ball.hpp"
#include "types.hpp"

class Scene
{
private:
    std::list<Object*> objects_;
    Camera camera_;

public:
    Scene() = default;
    Scene(Camera camera, std::list<Object*> objects) : camera_(camera), objects_(objects) {}
    ~Scene() = default;

    // Default copying for now - list is copied (are the balls copied as well?)
    Scene& operator=(const Scene& that) = default;
    Scene(const Scene& that) = default;

    Camera getCamera() const { return camera_; }

    std::list<Object*> getObjects() const { return objects_; }
};

std::ostream &operator<<(std::ostream& out, const Scene& scene) {
    out << "\n========== SCENE INFORMATION ==========" << std::endl;
    for (auto object : scene.getObjects())
    {
        // Printing has to be done separately for each object type - there has to be a better way than this
        if (dynamic_cast<Ball*>(object))
        {
            out << *dynamic_cast<Ball*>(object);
        }
    }
    out << std::endl;
    auto cam = scene.getCamera();
    out << "Camera at: (" << cam.position.transpose() << ") looking at: (" << cam.direction.transpose() << ") with FOV: " << cam.fov / M_PI * 180 << " degrees" << std::endl;
    out << "=======================================\n" << std::endl;
    return out;
}
