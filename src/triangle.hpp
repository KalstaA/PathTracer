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

    /**
     * @brief Calculate whether a given ray collides with the triangle.
     * 
     * If the ray collides with the triangle and the collision is closer than the current smallest distance,
     * the "rayHit" data structure will be updated according to the collision.
     * 
     * @param ray ray whose collision will be checked
     * @param rayHit address of a Hit data structure
     * @param smallestDistance current smallest distance
     */
    void collision(Ray& ray, Hit &rayHit, float& smallestDistance) {
        //Check if the ray is on the same plane as the triangle
        Vector p = ray.direction.cross(e2);
        double x = e1.dot(p);
        if(x == 0) {
            return;
        }

        //Check
        double inv_x = 1/x;
        Vector s = ray.origin - a;
        double y = s.dot(p)*inv_x;
        if(y < 0 || y > 1) {
            return;
        }

        //Check
        Vector q = s.cross(e1);
        double z = ray.direction.dot(q)*inv_x;
        if(z < 0 || y+z > 1) {
            return;
        }
        
        //Calculate matrix columns 
        Vector A1 = a-b;
        Vector A2 = a-c;
        Vector A3 = ray.direction;
        Vector b = a-ray.origin;
        
        //Construct the matrix
        Matrix A;
        A << A1, A2, A3;
        double detA = A.determinant();

        //Use Cramer's rule to solve for beta, gamma and t

        Matrix beta_A;
        beta_A << b, A2, A3;
        double beta = beta_A.determinant()/detA;

        Matrix gamma_A;
        gamma_A << A1, b, A3;
        double gamma = gamma_A.determinant()/detA;

        Matrix t_A;
        t_A << A1, A2, b;
        double t = t_A.determinant()/detA;
        
        //Check if ray collided with the triangle and t is smaller than current smallest distance
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

    /**
     * @brief Print triangle info to the desired output stream.
     * 
     * @param out output stream
     */
    void printInfo(std::ostream& out) const {
        std::vector<Vector> v = getVertexPos();
        out << "Triangle at: (" << v[0].transpose() << "), (" << v[1].transpose() 
        << "), (" << v[2].transpose() << "), with normal: (" << getNormal().transpose() 
        << "), with material: " << getMaterial().name << std::endl;
    }

    /**
     * @brief Get the vertex positions of the triangle
     * 
     * @return std::vector<Vector> containing the vertice vectors
     */
    std::vector<Vector> getVertexPos() const {
        std::vector<Vector> v;
        v.push_back(a);
        v.push_back(b);
        v.push_back(c);
        return v;
    }

    /**
     * @brief Get the plane vectors of the triangle
     * 
     * @return std::vector<Vector> containing the plane vectors
     */
    std::vector<Vector> getPlaneVec() const {
        std::vector<Vector> v;
        v.push_back(e1);
        v.push_back(e2);
        return v;
    }

    /**
     * @brief Get the normal of the triangle
     * 
     * @return The normal vector of the triangle
     */
    Vector getNormal() const {
        return n;
    }

private:
    Vector a, b, c;
    Vector e1, e2;
    Vector n;
};


