#include <gtest/gtest.h>
#include "ball.hpp"
#include "types.hpp"

Material RED_DIFFUSE = { .color = Color(1, 0, 0), .name = "RED DIFFUSE" };

// Test GetRadius method
TEST(BALL, GetRadius) {
  Ball ball1(Vector(5, 0, 0), 1, RED_DIFFUSE);
  Ball ball2(Vector(5, 0, 0), 1.52, RED_DIFFUSE);
  Ball ball3(Vector(5, 0, 0), -1, RED_DIFFUSE);
  // Expect equality.
  EXPECT_EQ(1, ball1.getRadius());
  EXPECT_GE(1.52001, ball2.getRadius());
  EXPECT_LE(1.51999, ball2.getRadius());
  EXPECT_EQ(-1, ball3.getRadius());
}
