#pragma once

#include "types.hpp"
#include "object.hpp"
#include <vector>

/**
 * @brief Representation of a mathematical triangle object in the scene
 * 
 */
class Triangle : public Object
{
public:
    Triangle(Vector v0, Vector v1, Vector v2, Material m) : Object(v0, m) {
        //Vectors pointing at the vertices
        a = v0;
        b = v1;
        c = v2;

        //Vectors on the triangle plane
        e1 = b-a;
        e2 = c-a;

        //Normal vector of the triangle
        n = e1.cross(e2).normalized();

    }

    void collision(Ray& ray, Hit &rayHit, float& smallestDistance) {
        
        Vector p = ray.direction.cross(e2);
        double x = e1.dot(p);
        if(x == 0) {
            return;
        }
        double inv_x = 1/x;
        Vector t_ = ray.origin - a;
        double u = t_.dot(p)*inv_x;
        if(u < 0 || u > 1) {
            return;
        }
        Vector q = t_.cross(e1);
        double v = ray.direction.dot(q)*inv_x;
        if(v < 0 || u+v > 1) {
            return;
        }
        
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
};


