#pragma once 

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Gui {
    public:
        Gui(int res_x, int res_y) : res_x_(res_x), res_y_(res_y) {}

        void openwindow() {
            sf::RenderWindow window(sf::VideoMode(res_x_, res_y_), "Path Tracer", sf::Style::Close);
            window.setSize(sf::Vector2u(res_x_, res_y_));
            //window.setVerticalSyncEnabled(true);
            window.setFramerateLimit(0);
            sf::Image image; 
            image.loadFromFile("image.png");
            sf::Texture texture;
            texture.loadFromImage(image);  
            sf::Sprite sprite;
            sprite.setTexture(texture);

            while (window.isOpen())
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();
                }
                window.clear();
                window.draw(sprite);
                window.display();
            }
                }

    private:
        int res_x_;
        int res_y_;
};