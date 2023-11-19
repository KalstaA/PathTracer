#include <gtest/gtest.h>
#include <stdlib.h>
#include "interface.hpp"
#include "types.hpp"

Interface interface;

// Test createImg method
TEST(BALL, GetRadius) {
  std::vector<std::vector<Color>> pixels;
  std::vector<Color> row1;
  std::vector<Color> row2;
  row1.push_back(Color(0.1, 0.2, 0.3));
  row1.push_back(Color(1.0, 0.9, 0.8));
  row1.push_back(Color(0.4, 0.5, 0.6));
  row1.push_back(Color(0.7, 0.6, 0.5));
  pixels.push_back(row1);
  pixels.push_back(row2);
  interface.createImg(pixels);
  sf::Image img = interface.getImage();
  EXPECT_EQ(img.getPixel(0,0), {25, 51, 76, 0});
}