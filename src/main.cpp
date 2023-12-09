#include "ball.hpp"
#include "scene.hpp"
#include "renderer.hpp"
#include "interface.hpp"
#include "fileloader.hpp"
#include "fileloader_ex.hpp"
#include "gui.hpp"
#include "gui_ex.hpp"
#include "triangle.hpp"
#include "trianglemesh.hpp"
#include "types.hpp"

#include <iostream>
#include <exception>
#include <string>
#include <cstdlib>

int main(int argc, char *argv[]) {

  try
  {
    if (argc == 1)
    {
      Gui gui;
      gui.openSettings(gui.titleScreen());
    }
    else if (argc == 7)
    {
      std::string filePath = argv[1];

      int resX = std::stoi(argv[2], nullptr);
      int resY = std::stoi(argv[3], nullptr); 
      int samples = std::stoi(argv[4], nullptr);
      int bounces = std::stoi(argv[5], nullptr);
      std::string filename = argv[6];

      FileLoader test(filePath);
      std::shared_ptr<Scene> testScene = test.loadSceneFile();
      std::cout << (*testScene);

      Renderer testRenderer(resX, resY, testScene);
      testRenderer.setMaxBounces(bounces);

      auto result = testRenderer.parallelRender(samples);

      Interface interface;
      interface.createImg(result);
      bool imgSaved = interface.saveImage(filename);
      if (imgSaved)
      {
        std::cout << "Image saved succesfully" << std::endl;
      }
      else
      {
        std::cout << "Saving image failed" << std::endl;
      }
    }
    else
    {
      std::cout << "Invalid command line arguments." << std::endl;
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
  catch (std::invalid_argument& ex)
  {
    std::cout << "Invalid command line arguments." << std::endl;
    std::cout << ex.what() << std::endl;
  }
  catch (GuiException& ex)
  {
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
