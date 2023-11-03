#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <SFML/Graphics.hpp>
#include <../libs/Eigen/Dense>

#include <list>
#include <random>
#include <vector>

#include "types.hpp"
#include "object.hpp"
#include "ball.hpp"
#include "scene.hpp"
#include "renderer.hpp"

int main() {

  Camera testCam;
  testCam.position = Vector(0, 0, 0);
  testCam.direction = Vector(1, 0, 0);
  testCam.fov = M_PI / 3;
  testCam.up = Vector(0, 0, 1);
  testCam.right = Vector(0, -1, 0);
  testCam.focus_distance = 5;

  Material testMaterial;
  testMaterial.color = Color(1, 0, 0);
  testMaterial.emission_color = Color(0, 0, 0);
  testMaterial.emission_strength = 0.0f;
  testMaterial.specularity = 0.5;

  Ball ball1(Vector(5, 0, 0), 1, testMaterial);
  std::list<Ball> testBalls;
  testBalls.push_back(ball1);

  Scene testScene(testCam, testBalls);

  int resX = 300, resY = 170;
  Renderer testRenderer(resX, resY, testScene);
  auto result = testRenderer.render();

  for (int y = 0; y < resY; y++)
  {
    for (int x = 0; x < resX; x++)
    {
      char symbol = result[x][y].lpNorm<1>() > 1.0 ? '#' : '.';
      std::cout << symbol << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "Hello world" << std::endl;
  sf::Image img;
  img.create(100, 100);
  img.saveToFile("image.png");
  return 0;
}

