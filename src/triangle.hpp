#pragma once

#include "types.hpp"
#include "object.hpp"
#include <vector>


struct Vertex 
{
public:
    Vector pos;
    Vector ng;
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

        uv[0] = v0.uv;
        uv[1] = v1.uv;
        uv[2] = v2.uv;

        N[0] = v0.ng;
        N[1] = v1.ng;
        N[2] = v2.ng;
    }

    void collision(Ray& ray, Hit &rayHit, float& smallestDistance) {
        Vector A1 = a-b;
        Vector A2 = a-c;
        Vector A3 = ray.direction;
        Vector b = a-ray.origin;
        
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
        

        if((beta + gamma < 1) && (beta > 0) && (gamma > 0) && t < smallestDistance) {
            smallestDistance = t;
            rayHit.distance = t;
            rayHit.material = this->getMaterial();
            rayHit.did_hit = true;
            rayHit.point = ray.origin + ray.direction*t;
            rayHit.normal = this->n;
        }

        return;
    }

    void printInfo(std::ostream& out) const {
        std::vector<Vector> v = getVertexPos();
        out << "Triangle at: (" << v[0].transpose() << "), (" << v[1].transpose() << "), (" << v[2].transpose() << "), with normal: (" << getNormal().transpose() << "), with material: " << getMaterial().name << std::endl;
    }

    std::vector<Vector> getVertexPos() const {
        std::vector<Vector> v;
        v.push_back(a);
        v.push_back(b);
        v.push_back(c);
        return v;
    }

    Vector getNormal() const {
        return n;
    }

private:
    Vector a, b, c;
    Vector e1, e2;
    Vector n;
    Vector N[3];
    Vector2 uv[3];
};


