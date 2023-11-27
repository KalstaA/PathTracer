#pragma once 

#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include "button.hpp"
#include "textbox.hpp"
#include "fileloader.hpp"

class Gui : public Interface {
public:
    Gui(int res_x, int res_y, Renderer &gui_fileloader) : res_x_(res_x), res_y_(res_y), gui_fileloader_(gui_fileloader) {}


    void titleScreen() {
        sf::Font arial;   
        arial.loadFromFile("../src/Arial.ttf");
        sf::Font comic;
        comic.loadFromFile("../src/Comic Sans MS.ttf");
        sf::RenderWindow window(sf::VideoMode(700, 400), "Path Tracer", sf::Style::Titlebar | sf::Style::Close);

        sf::Text title;
        title.setFillColor(sf::Color::White);
        title.setFont(comic);
        title.setCharacterSize(50);
        title.setString("Path Tracer");
        title.setPosition({175, 150});


        Textbox filepathBox(25, sf::Color::White, false, 30, "Enter filename ");
        filepathBox.setFont(arial);
        filepathBox.setPos({175, 225});

        while(window.isOpen()) {
            sf::Event event;

            while(window.pollEvent(event)) {
                switch(event.type) {
                    case sf::Event::Closed:
                        window.close();
                    case sf::Event::MouseButtonPressed:
                        if(filepathBox.onButton(window)) {
                            filepathBox.setSelected();
                            std::cout << "Click" << std::endl;
                        }
                    case sf::Event::TextEntered:
                        filepathBox.typedOn(event);
                    case sf::Event::KeyPressed:
                        if(event.key.code == sf::Keyboard::Enter){ 
                            std::string filepath = filepathBox.getInput().substr();
                            std::cout << filepath << std::endl;
                            FileLoader loader(filepath);
                            window.close();
                            openSettings(loader.loadSceneFile());
                        }
                        
                }
            } 

            window.clear();
            window.draw(title);
            filepathBox.draw(window);
            window.display();

        }
    }

    void openSettings(Scene loadedScene) {
        FileLoader sceneLoader("Empty");
        sf::Image image; 
        sf::Sprite sprite;
        sf::Texture texture;
        sf::Font arial;
        if(!arial.loadFromFile("../src/Arial.ttf")) {
            std::cout << "Error loading font" << std::endl;
        }

        sf::RenderWindow window(sf::VideoMode(700, 400), "Path Tracer", sf::Style::Titlebar | sf::Style::Close);
        
        
        sf::Vector2f size(200, 100);
        Button preview("Preview", sf::Color::Black, 20, size, sf::Color::Green);
        preview.setFont(arial);
        preview.setPos({0, 300});

        Textbox fovbox(20, sf::Color::White, false, 3, "FOV");
        fovbox.setFont(arial);
        fovbox.setPos({100, 100});
            
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
                            Renderer previewCreator(500, 400, loadedScene);
                            createImg(previewCreator.render());
                            saveImage("preview.png");
                            image.loadFromFile("preview.png");
                            texture.loadFromImage(image);  
                            sprite.setTexture(texture);
                            sprite.setPosition(200, 0);
                            }
                        if(fovbox.onButton(window)) {
                            fovbox.setSelected();
                            std::cout << "Click" << std::endl;
                        }
                    case sf::Event::TextEntered:
                        fovbox.typedOn(event);
                }
                }

            window.clear();
            preview.draw(window);
            window.draw(sprite);
            fovbox.draw(window);
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

private:
    int res_x_;
    int res_y_;
    Renderer gui_fileloader_;
};