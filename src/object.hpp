#pragma once

class Object
{
private:
    Vector position_;
    Material material_;
public:
    Object(Vector position, Material material) : position_(position), material_(material) {}
    ~Object() = default;

    // TO-DO copy constructor and copy assignment

    Vector getPosition() const { return position_; }
    Material getMaterial() const { return material_; }

    virtual Hit collision(Ray ray) = 0;
};