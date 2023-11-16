#pragma once

#include "object.hpp"


struct Vertex 
{
public:
    Vector pos;
    Vector n;
    Vector2 uv;
};


class Triangle : public Object
{
public:
    Triangle(Vertex v0, Vertex v1, Vertex v2, Material m) : Object(v0.pos, m) {
        a = v0.pos;
        b = v1.pos;
        c = v2.pos;

        e1 = b-a;
        e2 = c-a;

        n = e1.cross(e2).normalized();

        uv_[0] = v0.uv;
        uv_[1] = v1.uv;
        uv_[2] = v2.uv;

        N_[0] = v0.n;
        N_[1] = v1.n;
        N_[2] = v2.n;
    }

    void collision(Ray& ray, Hit &rayHit, float& smallestDistance) {
        Vector A1 = a-b;
        Vector A2 = a-c;
        Vector A3 = ray.direction;
        Vector b = a-ray.origin;
        
        /*
        Matrix A {
            {A1(0), A2(0), A3(0)},
            {A1(1), A2(1), A3(1)},
            {A1(2), A2(2), A3(2)}
        };

        Vector x = A.colPivHouseholderQr().solve(b);
        double beta = x(0);
        double gamma = x(1);
        double t = x(2);
        */
        
        Matrix A;
        A << A1, A2, A3;
        double detA = A.determinant();

        Matrix beta_A;
        beta_A << b, A2, A3;
        double beta = beta_A.determinant()/detA;

        Matrix gamma_A;
        gamma_A << A1, b, A3;
        double gamma = gamma_A.determinant()/detA;

        Matrix t_A;
        t_A << A1, A2, b;
        double t = t_A.determinant()/detA;
        

        if((beta + gamma < 1) && (beta > 0) && (gamma > 0) && (float)t < smallestDistance) {
            smallestDistance = t;
            rayHit.distance = t;
            rayHit.material = this->getMaterial();
            rayHit.did_hit = true;
            rayHit.point = ray.origin + ray.direction*t;
            rayHit.normal = this->n;
        }

        return;
    }

private:
    Vector a, b, c;
    Vector e1, e2;
    Vector n;
    Vector N_[3];
    Vector2 uv_[3];
};

