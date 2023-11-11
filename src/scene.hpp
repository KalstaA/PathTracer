#pragma once

#include <list>
#include "ball.hpp"
#include "types.hpp"
#include "environment.hpp"

/**
 * @brief Representation of a 3D-scene
 * 
 */
class Scene
{
private:
    std::list<Object*> objects_;
    Camera camera_;
    Environment environment_;

public:
    /**
     * @brief Construct an empty scene.
     * 
     */
    Scene() = default;

    /**
     * @brief Construct a new Scene object with a camera and some objects.
     * 
     * @param camera 
     * @param objects 
     */
    Scene(Camera camera, std::list<Object*> objects) : camera_(camera), objects_(objects) {}
    
    /* Object pointers cannot be simply deleted since they are not always allocated with new.
    * However, sometimes they can be dynamically allocated and not deleting them leads to a memory leak.
    * A possible solution is to use smart pointers and allow Object construction only using them.
    */

    ~Scene() { }

    // Default copying for now - list is copied (are the objects copied as well?)
    Scene& operator=(const Scene& that) = default;
    Scene(const Scene& that) = default;

    Camera getCamera() const { return camera_; }

    Environment& getEnvironment() { return environment_; }

    std::list<Object*> getObjects() const { return objects_; }
};

/**
 * @brief Print scene info to the desired output stream.
 * 
 * @param out output stream
 * @param scene scene to be printed
 * @return std::ostream& the output stream
 */
std::ostream &operator<<(std::ostream& out, const Scene& scene) {
    out << "\n==================== SCENE INFORMATION ====================" << std::endl;
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
    out << "===========================================================\n" << std::endl;
    return out;
}
