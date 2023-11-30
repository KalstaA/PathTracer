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
    /**
     * @brief Construct a new Triangle object
     * 
     * @param v0 Vertex 1
     * @param v1 Vertex 2
     * @param v2 Vertex 3
     * @param m Material of the triangle
     */
    Triangle(Vector v0, Vector v1, Vector v2, std::shared_ptr<Material> m) : Object(v0, m) {
        //Vectors pointing at the vertices
        a = v0;
        b = v1;
        c = v2;

        //Vectors on the triangle plane
        e1 = b-a;
        e2 = c-a;

        //Normal vector of the triangle
        n = e1.cross(e2).normalized();

        //Centroid of the triangle
        centroid = Vector(v0.mean(), v1.mean(), v2.mean());
    }

    /**
     * @brief Calculate whether a given ray collides with the triangle using the Möller-Trumbore algorithm
     * 
     * If the ray collides with the triangle and the collision is closer than the current smallest distance,
     * the "rayHit" data structure will be updated according to the collision.
     * 
     * @param ray ray whose collision will be checked
     * @param rayHit address of a Hit data structure
     * @param smallestDistance current smallest distance
     */
    void collision(Ray& ray, Hit &rayHit, float& smallestDistance) {
        //Compute determinant
        Vector p = ray.direction.cross(e2);
        float det = e1.dot(p);

        //Check if the ray is on the same plane as the triangle
        if(det == 0) return;

        float invDet = 1 / det;

        //Compute beta
        Vector s = ray.origin - a;
        float beta = s.dot(p)*invDet;

        //If beta < 0 or beta > 1 the ray does not intersect the triangle
        if(beta < 0 || beta > 1) return;

        //Compute gamma
        Vector q = s.cross(e1);
        float gamma = ray.direction.dot(q)*invDet;

        //If gamma < 0 or beta + gamma > 1 the ray does not intersect the triangle
        if(gamma < 0 || beta + gamma > 1) return;

        //Compute t
        float t = e2.dot(q)*invDet;

        //If t < smallestDistance the ray intersects the triangle
        if(t < smallestDistance) {
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
        << "), with material: " << getMaterial()->getName() << std::endl;
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

    /**
     * @brief Get the centroid of the triangle
     * 
     * @return The centroid vector of the triangle
     */
    Vector getCentroid() const {
        return centroid;
    }

private:
    Vector a, b, c;
    Vector e1, e2;
    Vector n;
    Vector centroid;
};


