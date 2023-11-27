#include "ball.hpp"
#include "scene.hpp"
#include "renderer.hpp"
#include "interface.hpp"
#include "fileloader.hpp"
#include <iostream>
#include "gui.hpp"

int main() {




  FileLoader test("../src/scene.yaml");
  Scene testScene = test.loadSceneFile();
  std::cout << testScene;

  int resX = 500, resY = 400;
  Renderer testRenderer(resX, resY, testScene);

  Gui testgui(resX, resY, testRenderer);
  testgui.titleScreen();
  //testgui.openwindow("image.png");
/*
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
  }*/


  return 0;
}