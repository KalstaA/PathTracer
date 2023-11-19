#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <SFML/Graphics.hpp>

#include "ball.hpp"
#include "scene.hpp"
#include "renderer.hpp"
#include "interface.hpp"
#include "FileLoader.hpp"

int main() {

  FileLoader test("scene.yaml");
  Scene testScene = test.loadSceneFile();
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

