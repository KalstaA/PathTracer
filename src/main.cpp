#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <SFML/Graphics.hpp>

#include "ball.hpp"
#include "scene.hpp"
#include "renderer.hpp"
#include "interface.hpp"
#include "triangle.hpp"

int main() {

  Camera testCam = {
    .position = Vector(0, 0, 0),
    .direction = Vector(1, 0, 0),
    .fov = M_PI / 3,
    .up = Vector(0, 0, 1),
    .focus_distance = 5
  };  

  Material RED_DIFFUSE = { .color = Color(1, 0, 0), .name = "RED DIFFUSE" };

  Material GREY_DIFFUSE = { .color = Color(0.5, 0.5, 0.5), .name = "GREY DIFFUSE" };

  Material GREEN_DIFFUSE = { .color = Color(0, 1, 0), .name = "GREEN DIFFUSE" };

  Material BLUE_DIFFUSE = { .color = Color(0, 0, 1), .name = "BLUE DIFFUSE" };

  Material MIRROR = { .specularity = 1.0, .name = "MIRROR" };

  Material SUN = {
    .color = Color(1, 1, 0),
    .emission_color = Color(1, 1, 1),
    .emission_strength = 0.5,
    .name = "SUN"
  };

  Ball ball1(Vector(5, 0, 0), 1, RED_DIFFUSE);
  Ball ball2(Vector(6, 0, -31), 30, GREY_DIFFUSE);
  Ball ball3(Vector(10, -30, 20), 20, SUN);
  Ball ball4(Vector(4, -0.5, -0.5), 0.5, GREEN_DIFFUSE);
  Ball ball5(Vector(6, 2, -0.5), 0.5, BLUE_DIFFUSE);
  Ball ball6(Vector(6, 1.5, 1), 0.7, MIRROR);
  
  Vertex v0 = {
    .pos = Vector(4,-1.5,0)
  };
  Vertex v1 = {
    .pos = Vector(5,-1,0)
  };
  Vertex v2 = {
    .pos = Vector(4,-1.5,1)
  };

  Triangle t1(v0, v1, v2, GREEN_DIFFUSE);

  std::list<Object*> testBalls;
  testBalls.push_back(&ball1);
  testBalls.push_back(&ball2);
  testBalls.push_back(&ball3);
  testBalls.push_back(&ball4);
  testBalls.push_back(&ball5);
  testBalls.push_back(&ball6);
  
  testBalls.push_back(&t1);

  Scene testScene(testCam, testBalls);
  testScene.getEnvironment().setSky();
  std::cout << testScene;

  int resX = 500, resY = 400;
  Renderer testRenderer(resX, resY, testScene);
  auto result = testRenderer.render();

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

