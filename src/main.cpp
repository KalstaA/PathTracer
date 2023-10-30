#include <iostream>
#include <SFML/Graphics.hpp>
#include <../libs/Eigen/Dense>

#include "types.hpp"
#include "object.hpp"
#include "scene.hpp"
#include "renderer.hpp"

int main() {
  std::cout << "Hello world" << std::endl;
  sf::Image img;
  img.create(100, 100);
  img.saveToFile("image.png");
  return 0;
}
