#include "ball.hpp"
#include "scene.hpp"
#include "renderer.hpp"
#include "interface.hpp"
#include "fileloader.hpp"
#include "fileloader_ex.hpp"
// #include <yaml-cpp/exception.h>

#include <iostream>
#include <exception>

int main() {
  try
  {
    FileLoader test("../src/scene.yaml");
    std::shared_ptr<Scene> testScene = test.loadSceneFile();
    std::cout << (*testScene);

    int resX = 400, resY = 300;
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
  }
  catch (FileLoaderException& ex)
  {
    std::cout << ex.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch (YAML::Exception& ex)
  {
    std::cout << "Exception caught:" << std::endl;
    std::cout << ex.what() << std::endl;
    return EXIT_FAILURE;

  }
  return EXIT_SUCCESS;
}
