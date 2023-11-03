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
#include "interface.hpp"

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

  //Ball ball1(Vector(5, 0, 0), 1, testMaterial);
  Ball ball2(Vector(10, 1, 1), 1, testMaterial);
  std::list<Ball> testBalls;
  testBalls.push_back(ball2);

  Scene testScene(testCam, testBalls);

  int resX = 300, resY = 170;
  Renderer testRenderer(resX, resY, testScene);
  auto result = testRenderer.render();

  std::cout << "Hello world" << std::endl;

  Interface interface;
  interface.createImg(result);
  bool imgSaved = interface.saveImage("image.png");
  if (imgSaved)
  {
    std::cout << "Image saved succesfully" << std::endl;
  }
  else
  {
    std::cout << "Saving image failed" << std::endl;
  }

  return 0;
}

