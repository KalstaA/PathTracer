#pragma once

#include "types.hpp"
#include "triangle.hpp"
#include <vector>

struct AABB
{
    Vector min, max;
};

struct Node
{
    AABB box;
    int leftChild;
    bool isLeaf() { return triCount > 0; }
    int firstTriIdx, triCount;
};

/**
 * @brief Bounding volume hierarchy class for optimising TriangleMesh collisions.
 * Creates a data structure that divides the triangles of the mesh in to groups of 
 * Axis Aligned Bounding Boxes (AABB). If a ray misses a box then none of the triangles
 * in the box need to be checked for a collision.
 */
class BVH
{
public:
    /**
     * @brief Default constructor for BVH
     * 
     */
    BVH() { }
    
    /**
     * @brief Construct a new BVH object
     * 
     * @param tris vector containing all the triangles in the mesh
     */
    BVH(std::vector<Triangle> tris) {
        //Initialize variables
        n = tris.size();
        triangles = tris;
        std::vector<Node> bvhNodes(n*2 - 1);
        rootNodeIdx = 0;
        nodesUsed = 1;
        for(int i = 0; i < n; i++) {
            triIdx.push_back(i);
        }
        Node& root = bvhNodes[rootNodeIdx];
        root.leftChild = 0;
        root.triCount = n;

        //Create the structure
        UpdateNodeBounds(bvhNodes, rootNodeIdx);
        Subdivide(bvhNodes, rootNodeIdx);

        //Copy structure to a private variable
        for(int i = 0; i < bvhNodes.size(); i++) {
            nodes.push_back(bvhNodes[i]);
        }
    }

    /**
     * @brief Updates the bounds of the AABB based on the triangles it contains
     * 
     * @param bvhNodes vector containing the nodes
     * @param nodeIdx the current node index
     */
    void UpdateNodeBounds(std::vector<Node>& bvhNodes, int nodeIdx) {
        Node& currentNode = bvhNodes[nodeIdx];

        //Set bounds to "infinity" in the beginning
        currentNode.box.min = Vector(1e30f, 1e30f, 1e30f);
        currentNode.box.max = Vector(-1e30f, -1e30f, -1e30f);

        //Loop over triangles and set new bounds accordingly
        for(int first = currentNode.firstTriIdx, i=0; i < currentNode.triCount; i++) {
            int leafTriIdx = triIdx[first + i];
            Triangle &leafTriangle = triangles[leafTriIdx];
            std::vector<Vector> vertices = leafTriangle.getVertexPos();
            currentNode.box.min = currentNode.box.min.cwiseMin(vertices[0]);
            currentNode.box.min = currentNode.box.min.cwiseMin(vertices[1]);
            currentNode.box.min = currentNode.box.min.cwiseMin(vertices[2]);
            currentNode.box.max = currentNode.box.max.cwiseMax(vertices[0]);
            currentNode.box.max = currentNode.box.max.cwiseMax(vertices[1]);
            currentNode.box.max = currentNode.box.max.cwiseMax(vertices[2]);
        }
    }

    /**
     * @brief Divides the traingles of the current node into new AABBs recursively
     * 
     * @param bvhNodes vector containing the nodes
     * @param nodeIdx current node index
     */
    void Subdivide(std::vector<Node>& bvhNodes, int nodeIdx) {
        //Terminates recursion
        Node &currentNode = bvhNodes[nodeIdx];
        if(currentNode.triCount <= 2) return;

        //Determine the split
        Vector extent = currentNode.box.max - currentNode.box.min;
        int axis = 0;
        if(extent(1) > extent(0)) axis = 1;
        if(extent(2) > extent(axis)) axis = 2;
        float splitPos = currentNode.box.min(axis) + extent(axis)*0.5f;

        //Partition triangles
        int i = currentNode.firstTriIdx;
        int j = i + currentNode.triCount - 1;
        while(i <= j) {
            if(triangles[triIdx[i]].getCentroid()(axis) < splitPos) {
                i++;
            }else {
                std::swap(triIdx[i], triIdx[j--]);
            }
        }

        //Check if one of the sides is empty
        int leftCount = i - currentNode.firstTriIdx;
        if(leftCount == 0 || leftCount == currentNode.triCount) return;

        //create child nodes
        int leftChildIdx = nodesUsed++;
        int rightChildIdx = nodesUsed++;
        bvhNodes[leftChildIdx].firstTriIdx = currentNode.firstTriIdx;
        bvhNodes[leftChildIdx].triCount = leftCount;
        bvhNodes[rightChildIdx].firstTriIdx = i;
        bvhNodes[rightChildIdx].triCount = currentNode.triCount - leftCount;
        currentNode.leftChild = leftChildIdx;
        currentNode.triCount = 0;
        
        //Update node bounds
        UpdateNodeBounds(bvhNodes, leftChildIdx);
        UpdateNodeBounds(bvhNodes, rightChildIdx);
        //Recursion
        Subdivide(bvhNodes, leftChildIdx);
        Subdivide(bvhNodes, rightChildIdx);
    }

    /**
     * @brief Calculate whether a given ray collides with the TriangleMesh contained in the BVH recursively
     * 
     * @param ray ray whose collision will be checked
     * @param rayHit address of a Hit data structure
     * @param smallestDistance current smallest distance
     * @param nodeIdx current node index
     */
    void BVHCollision(Ray& ray, Hit& rayHit, float& smallestDistance, const int nodeIdx) {
        Node& currentNode = nodes[nodeIdx];
        if(!AABBCollision(currentNode.box, ray, smallestDistance)) return;
        if(currentNode.isLeaf()) {
            for(int i = 0; i < currentNode.triCount; i++) {
                triangles[triIdx[currentNode.firstTriIdx + i]].collision(ray, rayHit, smallestDistance);
            }
        }else {
            BVHCollision(ray, rayHit, smallestDistance, currentNode.leftChild);
            BVHCollision(ray, rayHit, smallestDistance, currentNode.leftChild + 1);
        }
    }

    /**
     * @brief Calculate whether a given ray collides with the AABB
     * 
     * @param box the AABB for which collision is tested
     * @param ray ray whose collision will be checked
     * @param smallestDistance current smallest distance
     * @return true If the ray collides with the box
     * @return false If the ray does not collide with the boc
     */
    bool AABBCollision(AABB box, Ray ray, float smallestDistance) {
        float tx1 = (box.min(0) - ray.origin(0))/ray.direction(0);
        float tx2 = (box.max(0) - ray.origin(0))/ray.direction(0);
        float tmin = std::min(tx1, tx2);
        float tmax = std::max(tx1, tx2);

        float ty1 = (box.min(1) - ray.origin(1))/ray.direction(1);
        float ty2 = (box.max(1) - ray.origin(1))/ray.direction(1);
        tmin = std::max(tmin, std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));

        float tz1 = (box.min(2) - ray.origin(2))/ray.direction(2);
        float tz2 = (box.max(2) - ray.origin(2))/ray.direction(2);
        tmin = std::max(tmin, std::min(tz1, tz2));
        tmax = std::min(tmax, std::max(tz1, tz2));

        return tmax >= tmin && tmin < smallestDistance && tmax > 0;
    }

    /**
     * @brief Get the RootNodeIdx of the BVH
     * 
     * @return int
     */
    int getRootNodeIdx() const {
        return rootNodeIdx;
    }

private:
    std::vector<Triangle> triangles;
    std::vector<int> triIdx;
    std::vector<Node> nodes;
    int rootNodeIdx;
    int nodesUsed;
    int n;
};
