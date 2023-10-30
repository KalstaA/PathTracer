#include <list>

class Scene
{
private:
    std::list<Object> objects_;
    Camera camera_;

public:
    Scene(/* args */);
    ~Scene();

    Camera getCamera() {
        return camera_;
    }
};
