class Scene
{
private:
    std::list<Ball> balls_;
    Camera camera_;

public:
    Scene(Camera camera, std::list<Ball> balls) : camera_(camera), balls_(balls) {}
    ~Scene() = default;

    // TO-DO copy constructor and copy assigment

    Camera getCamera() {
        return camera_;
    }

    std::list<Ball> getBalls() {
        return balls_;
    }
};
