#include "ball.hpp"
#include "scene.hpp"
#include "renderer.hpp"
#include "interface.hpp"
#include "fileloader.hpp"
#include "fileloader_ex.hpp"
//#include "gui.hpp"
#include "triangle.hpp"
#include "trianglemesh.hpp"
#include "types.hpp"

#include <iostream>
#include <exception>

int main() {
  try
  {
    //Gui gui;
    //gui.titleScreen();
    FileLoader test("../src/scenes/test.yaml");
    std::shared_ptr<Scene> testScene = test.loadSceneFile();
    std::cout << (*testScene);
    int rays_per_pixel = 5;

    int resX = 400, resY = 300;
    Renderer testRenderer(resX, resY, testScene);

    auto result = testRenderer.parallelRender(rays_per_pixel);

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
    std::cout << "YAML exception caught:" << std::endl;
    std::cout << ex.what() << std::endl;
    return EXIT_FAILURE;

  }
  catch (std::exception& ex)
  {
    std::cout << "Standard library exception caught: " << std::endl;
    std::cout << ex.what() << std::endl;
  }
  catch (...)
  {
    std::cout << "Unknown exception" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
