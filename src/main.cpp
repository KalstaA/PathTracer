#include <iostream>
#include "message.h"
#include <SFML/Graphics.hpp>

int main() {
  Message m;
  m.printMessage();
  sf::Image img;
  img.create(100, 100);
  img.saveToFile("image.png");
  return 0;
}
