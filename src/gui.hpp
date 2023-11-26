#pragma once 

#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include "button.hpp"
#include "textbox.hpp"
#include "fileloader.hpp"

class Gui : public Interface {
public:
    Gui(int res_x, int res_y, Renderer &gui_fileloader) : res_x_(res_x), res_y_(res_y), gui_fileloader_(gui_fileloader) {}

    void opensettings() {
        FileLoader sceneLoader("Empty");
        sf::Image image; 
        sf::Sprite sprite;
        sf::Texture texture;
        sf::Font arial;
        arial.loadFromFile("Arial.ttf");

        sf::RenderWindow window(sf::VideoMode(700, 400), "Path Tracer", sf::Style::Titlebar | sf::Style::Close);
        
        
        sf::Vector2f size(200, 100);
        Button preview("Preview", sf::Color::Black, 200, size, sf::Color::Green);
        preview.setFont(arial);
        preview.setPos({0, 300});

        Textbox fovbox(280, sf::Color::White, false, 3);
        fovbox.setFont(arial);
        fovbox.setPos({0, 100});
            
        sf::Event event; 


        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {   
                switch(event.type) {
                    case sf::Event::Closed:
                        window.close();
                    case sf::Event::MouseMoved:
                        if(preview.onButton(window)) {
                            preview.setColor(sf::Color::White);
                            std::cout << "on button" << std::endl;
                        }else {
                            preview.setColor(sf::Color::Green);
                        }
                        break;
                    case sf::Event::MouseButtonPressed:
                        if(preview.onButton(window)) {
                            std::cout << "Clicker" << std::endl;
                            sceneLoader.setFilepath("../src/scene.yaml");
                            Renderer previewCreator(500, 400, sceneLoader.loadSceneFile());
                            createImg(previewCreator.render());
                            saveImage("preview.png");
                            image.loadFromFile("preview.png");
                            texture.loadFromImage(image);  
                            sprite.setTexture(texture);
                            sprite.setPosition(200, 0);
                            }
                    case sf::Event::TextEntered:
                        fovbox.typedOn(event);
                }
                }

            window.clear();
            preview.draw(window);
            window.draw(sprite);
            window.display();

            }

        }

    void openwindow(std::string filename) {
        sf::RenderWindow window(sf::VideoMode(res_x_, res_y_), "Path Tracer", sf::Style::Close);
        window.setSize(sf::Vector2u(res_x_, res_y_));
        window.setFramerateLimit(0);
        sf::Image image; 
        sf::Sprite sprite;
        sf::Texture texture;
        auto result = gui_fileloader_.parallelRender();

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {   
                switch(event.type) {
                    case sf::Event::Closed:
                        window.close();
                    case sf::Event::KeyPressed:
                        createImg(gui_fileloader_.Getpixels());
                        saveImage("image.png");
                        image.loadFromFile(filename);
                        texture.loadFromImage(image);  
                        sprite.setTexture(texture);
                        window.clear();
                        window.draw(sprite);
                        window.display();
                        }
                }
        createImg(result);
        saveImage("image.png");
        image.loadFromFile(filename);
        texture.loadFromImage(image);  
        sprite.setTexture(texture);
        window.clear();
        window.draw(sprite);
        window.display();
            }
        
        }

private:
    int res_x_;
    int res_y_;
    Renderer gui_fileloader_;
};