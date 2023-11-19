#include <gtest/gtest.h>
#include "environment.hpp"
#include "types.hpp"

// Demonstrate some basic assertions.
TEST(ENVIRONMENT, SkyColor) {
  Environment env;
  EXPECT_EQ(Color(0, 0, 0), env.getSkyColor());
  EXPECT_EQ(Color(0, 0, 0), env.getHorizonColor());
  EXPECT_EQ(Color(0, 0, 0), env.getGroundColor());
  env.setSky();
  EXPECT_EQ(Color(0.2, 0.5, 1.0), env.getSkyColor());
  EXPECT_EQ(Color(0.7, 0.8, 0.8), env.getHorizonColor());
  EXPECT_EQ(Color(0.1, 0.1, 0.1), env.getGroundColor());
  env.setSky(Color(0.2, 0.5, 1.0), Color(0.7, 0.8, 0.8), Color(0.2, 0.5, 1.0));
  EXPECT_EQ(Color(0.2, 0.5, 1.0), env.getGroundColor());
}
