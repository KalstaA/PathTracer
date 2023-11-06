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
//#include "FileLoader.hpp"

int main() { 

  YAML::Parser parser;
  Camera testCam;
  testCam.position = Vector(0, 0, 0);
  testCam.direction = Vector(1, 0, 0);
  testCam.fov = M_PI / 3;
  testCam.up = Vector(0, 0, 1);
  testCam.right = Vector(0, -1, 0);
  testCam.focus_distance = 5;

  Material testMaterial1;
  testMaterial1.color = Color(1, 0, 0);
  testMaterial1.emission_color = Color(0, 0, 0);
  testMaterial1.emission_strength = 0.0f;
  testMaterial1.specularity = 0.5;

  Material testMaterial2;
  testMaterial2.color = Color(0, 0, 0);
  testMaterial2.emission_color = Color(0, 0, 0);
  testMaterial2.emission_strength = 1000.0f;
  testMaterial2.specularity = 0.5;

  Ball ball1(Vector(5, 0, 0), 1, testMaterial1);
  Ball ball2(Vector(-10, 3, 3), 1, testMaterial2);
  std::list<Ball> testBalls;
  testBalls.push_back(ball1);
  testBalls.push_back(ball2);

  Scene testScene(testCam, testBalls);
  /*FileLoader test("scene.yaml");
  Scene testScene = test.loadSceneFile();*/

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

