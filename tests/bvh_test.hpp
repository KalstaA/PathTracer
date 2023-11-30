#include <gtest/gtest.h>
#include "triangle.hpp"
#include "types.hpp"
#include "trianglemesh.hpp"
#include "bvh.hpp"
#include "material.hpp"
#include <memory>

TEST(BVH, structure) {
    std::string knight_file = "../objects/knight.obj";
    TriangleMesh knight(knight_file, Vector(5,0,-1.5), RED_DIFFUSE, 1, 7*M_PI/4);
    //Expect equality
    EXPECT_GE(knight.getBVH().getTriangles().size()*2-1, knight.getBVH().getNodes().size());
}