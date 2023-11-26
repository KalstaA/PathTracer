#include "ball.hpp"
#include "scene.hpp"
#include "renderer.hpp"
#include "interface.hpp"
#include "fileloader.hpp"
#include <iostream>
#include "box.hpp"

int main() {

  Camera testCam = {
    .position = Vector(0, 2, 0),
    .direction = Vector(3, -1, 0).normalized(),
    .fov = M_PI / 3,
    .up = Vector(0, 0, 1),
    .focus_distance = 5
  };  

  Material RED_DIFFUSE = { .color = Color(1, 0, 0), .name = "RED DIFFUSE" };

  Box box1(Vector(3, 0, 0), 1, 1, 1, RED_DIFFUSE);

  std::list<Object*> testBoxes;
  testBoxes.push_back(&box1);

  Scene testScene(testCam, testBoxes);
  testScene.getEnvironment().setSky();
  std::cout << testScene;

  int resX = 500, resY = 400;
  Renderer testRenderer(resX, resY, testScene);

  auto result = testRenderer.parallelRender();

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