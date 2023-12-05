#include "ball.hpp"
#include "scene.hpp"
#include "renderer.hpp"
#include "interface.hpp"
#include "fileloader.hpp"
#include "fileloader_ex.hpp"
#include "gui.hpp"
#include "triangle.hpp"
#include "trianglemesh.hpp"
#include "types.hpp"

#include <iostream>
#include <exception>

int main(int argc, char *argv[]) {

  try
  {
    if (argc > 1 && strcmp(argv[1], "gui") == 0)
    {
      Gui gui;
      gui.titleScreen();
    }

    else
    {
      FileLoader test("../src/scenes/test2.yaml");
      std::shared_ptr<Scene> testScene = test.loadSceneFile();
      std::cout << (*testScene);

      int resX = 800, resY = 600;
      Renderer testRenderer(resX, resY, testScene);

      auto result = testRenderer.parallelRender(1);

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
