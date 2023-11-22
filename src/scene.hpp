#pragma once

#include <list>
#include <iostream>
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
    

    /**
     * @brief Destructor for Scene object. Deletes all the object in the scene.
     * 
     */
    ~Scene() {
        for (auto it = objects_.begin(); it != objects_.end(); ++it)
        {
            delete *it;
        }
    }

    // Default copying for now - list is copied (are the objects copied as well?)
    Scene& operator=(const Scene& that) = default;
    Scene(const Scene& that) = default;

    Camera getCamera() const { return camera_; }

    Environment& getEnvironment() { return environment_; }

    std::list<Object*> getObjects() const { return objects_; }

    /**
     * @brief Print scene info to the desired output stream.
     * 
     * @param out output stream
     * @param scene scene to be printed
     * @return std::ostream& the output stream
     */
    friend std::ostream &operator<<(std::ostream& out, const Scene& scene) {
        out << "\n========================= SCENE INFORMATION =========================" << std::endl;

        for (auto object : scene.objects_)
        {
            out << (*object);
        }

        out << std::endl;

        out << "Camera at: (" << scene.camera_.position.transpose() << ") looking at: (" << scene.camera_.direction.transpose() << ") with FOV: " << scene.camera_.fov / M_PI * 180 << " degrees" << std::endl;
        
        out << "=====================================================================\n" << std::endl;
        
        return out;
    }
};
