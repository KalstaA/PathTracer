#include <iostream>
#include <SFML/Graphics.hpp>
#include <../libs/Eigen/Dense>
#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>

int main() {
  sf::Image img;
  img.create(300, 200);
  img.saveToFile("image.png");
  Eigen::Vector3d vec(1.2, 1.4, 4.9);
  std::cout << vec << std::endl;
  YAML::Parser parser;
  return 0;
}
