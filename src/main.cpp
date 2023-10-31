#include <iostream>
#include <SFML/Graphics.hpp>
#include <../libs/Eigen/Dense>

#include <list>

#include "types.hpp"
#include "object.hpp"
#include "ball.hpp"
#include "scene.hpp"
#include "renderer.hpp"

int main() {

  Camera testCam;
  testCam.position = Vector(0, 0, 0);
  testCam.direction = Vector(1, 0, 0);
  testCam.fov = M_PI / 3;
  testCam.up = Vector(0, 0, 1);
  testCam.right = Vector(0, -1, 0);
  testCam.focus_distance = 5;

  Ball ball1(Vector(5, 0, 0), 1, Material(Color(1, 0, 0), 0.0f, Color(0, 0, 0)));
  std::list<Ball> testBalls;
  testBalls.push_back(ball1);

  Scene test(testCam, testBalls);

  std::cout << "Hello world" << std::endl;
  sf::Image img;
  img.create(100, 100);
  img.saveToFile("image.png");
  return 0;
}
