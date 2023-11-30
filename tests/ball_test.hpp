#include <gtest/gtest.h>
#include "ball.hpp"
#include "types.hpp"
#include "material.hpp"
#include <memory>

std::shared_ptr<Diffuse> RED_DIFFUSE = std::make_shared<Diffuse>(Color(1, 0, 0), "RED DIFFUSE", 0.0, Color(1.0, 1.0, 1.0));

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

// Test collision method
TEST(BALL, Collision) {
  // TODO
}
