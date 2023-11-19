#ifndef INTERFACE_TEST
#define INTERFACE_TEST

#include <gtest/gtest.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include "interface.hpp"
#include "types.hpp"

// Test createImg method
TEST(INTERFACE, CreateImg) {
  Interface interface;
  std::vector<Color> row1{ Color(0.1, 0.2, 0.3), Color(1.0, 0.9, 0.8) };
  std::vector<Color> row2{ Color(0.4, 0.5, 0.6), Color(0.7, 0.6, 0.5) };
  std::vector<std::vector<Color>> pixels{ row1, row2 };
  interface.createImg(pixels);
  sf::Image img = interface.getImage();
  sf::Color color1(25, 51, 76, 255);
  sf::Color color2(255, 229, 204, 255);
  sf::Color color3(102, 127, 153, 255);
  sf::Color color4(178, 153, 127, 255);
  EXPECT_EQ(img.getPixel(0, 0), color1);
  EXPECT_EQ(img.getPixel(0, 1), color2);
  EXPECT_EQ(img.getPixel(1, 0), color3);
  EXPECT_EQ(img.getPixel(1, 1), color4);
}

#endif