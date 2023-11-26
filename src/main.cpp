#include "ball.hpp"
#include "scene.hpp"
#include "renderer.hpp"
#include "interface.hpp"
#include "fileloader.hpp"
#include <iostream>
#include "box.hpp"

int main() {

  Camera testCam = {
    .position = Vector(0, 0, 0),
    .direction = Vector(1, 0, 0).normalized(),
    .fov = M_PI / 3,
    .up = Vector(0, 0, 1),
    .focus_distance = 5
  };  

  Material RED_DIFFUSE = { .color = Color(1, 0, 0), .name = "RED DIFFUSE" };
  Material CLEARCOAT = { .color = Color(1, 0, 0), .clearcoat = 0.5, .name = "CLEARCOAT" };

  Box box1(Vector(3, 0, 0), 1, 1, 1, CLEARCOAT);
  box1.rotate(M_PI / 4, Vector::UnitZ());
  box1.rotate(-M_PI / 8, Vector::UnitY());

  std::list<Object*> testBoxes;
  testBoxes.push_back(&box1);

  Scene testScene(testCam, testBoxes);
  testScene.getEnvironment().setSky();
  std::cout << testScene;

  int resX = 500, resY = 400;
  Renderer testRenderer(resX, resY, testScene);

  auto result = testRenderer.parallelRender(20);

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