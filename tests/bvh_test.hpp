#include <gtest/gtest.h>
#include "triangle.hpp"
#include "types.hpp"
#include "trianglemesh.hpp"
#include "bvh.hpp"

TEST(BVH, structure) {
    std::string knight_file = "../objects/knight.obj";
    TriangleMesh knight(knight_file, Vector(5,0,-1.5), RED_DIFFUSE, 1, 7*M_PI/4);
    //Expect equality
    EXPECT_EQ(knight.getBVH().getTriangles().size(), knight.getBVH().getNodes().size());
}