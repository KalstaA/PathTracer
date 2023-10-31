class Ball : public Object
{
private:
    Vector position_;
    float radius_;
    Material material_;

public:
    Ball(Vector position, float radius, Material material) : position_(position), radius_(radius), material_(material) {}
    ~Ball() = default;

    Point getPosition() {
        return position_;
    }

    float getRadius() {
        return radius_;
    }

    Material getMaterial() {
        return material_;
    }
};
