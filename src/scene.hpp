#pragma once

class Scene
{
private:
    std::list<Ball> balls_;
    Camera camera_;

public:

    Scene() = default;
    Scene(Camera camera, std::list<Ball> balls) : camera_(camera), balls_(balls) {}
    ~Scene() = default;

    // TO-DO copy constructor and copy assigment

    Camera getCamera() const {
        return camera_;
    }

    std::list<Ball> getBalls() const {
        return balls_;
    }
};
