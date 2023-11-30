#include <gtest/gtest.h>
#include "triangle.hpp"
#include "types.hpp"
#include "material.hpp"
#include <memory>

// Test GetCentroid method
TEST(Triangle, GetCentroid) {
    Triangle t1(Vector(0,0,0), Vector(1,1,1), Vector(3,4,5), RED_DIFFUSE);
    Triangle t2(Vector(1,-2,0), Vector(5,1,3), Vector(3,-2,0), RED_DIFFUSE);
    Triangle t3(Vector(1,2,3), Vector(2,1,4), Vector(0,1,7), RED_DIFFUSE);
    //Expect equality
    EXPECT_EQ(Vector(0, 1, 4), t1.getCentroid());
    EXPECT_EQ(Vector(-1.0/3, 3, 1.0/3), t2.getCentroid());
    EXPECT_EQ(Vector(2, 7.0/3, 8.0/3), t3.getCentroid());
}