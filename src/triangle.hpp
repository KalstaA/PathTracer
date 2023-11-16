#pragma once

#include "object.hpp"


struct Vertex {
public:
    Vector pos;
    Vector n;
    Vector2 uv:
};


class Triangle : public Object
{
public:
    Triangle(Vertex v0, Vertex v1, Vertex v2) {
        v0_ = v0.pos;
        v1_ = v1.pos;
        v2_ = v1.pos;

        e1_ = v1_ - v0_;
        e2_ = v2_ - v0_;
        n_ = e1_.cross(e2_).norm();

        uv_[0] = v0.uv;
        uv_[1] = v1.uv;
        uv_[2] = v2.uv;

        N_[0] = v0.n;
        N_[1] = v1.n;
        N_[2] = v2.n;
    }

    Vector GetBarycentric(Vector& p) const;

private:
    Vector v1_, v2_, v3_;
    Vector e1_, e2_;
    Vector n_;
    Vector N_[3]
    Vector2 uv_[3]
};

