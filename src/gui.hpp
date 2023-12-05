#pragma once 

#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include "button.hpp"
#include "textbox.hpp"
#include "fileloader.hpp"
#include "types.hpp"

class Gui : public Interface {
public:
    void titleScreen() {
        sf::Font arial;   
        arial.loadFromFile("../src/Arial.ttf");
        sf::Font comic;
        comic.loadFromFile("../src/ComicSansMS.ttf");
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
                        }
                    case sf::Event::TextEntered:
                        filepathBox.typedOn(event);
                    case sf::Event::KeyPressed:
                        if(event.key.code == sf::Keyboard::Enter){ 
                            std::string filepath = filepathBox.getInput().substr();
                            std::cout << filepath << std::endl;
                            FileLoader loader(filepath);
                            window.close();
                            std::shared_ptr<Scene> loadedScene = loader.loadSceneFile();
                            openSettings(loadedScene);
                        }
                        
                }
            } 

            window.clear();
            window.draw(title);
            filepathBox.draw(window);
            window.display();

        }
    }

    void openSettings(std::shared_ptr<Scene> loadedScene) {
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

        Textbox resXbox(20, sf::Color::White, false, 4, "ResX: ");
        resXbox.setFont(arial);
        resXbox.setPos({0, 0});

        Textbox resYbox(20, sf::Color::White, false, 4, "ResY: ");
        resYbox.setFont(arial);
        resYbox.setPos({0, 50});

        Textbox fovBox(20, sf::Color::White, false, 4, "Fov: ");
        fovBox.setFont(arial);
        fovBox.setPos({0, 100});

        Textbox dofBox(20, sf::Color::White, false, 4, "Depth of field: ");
        dofBox.setFont(arial);
        dofBox.setPos({0, 150});

        Textbox focusBox(20, sf::Color::White, false, 4, "Focus distance: ");
        focusBox.setFont(arial);
        focusBox.setPos({0, 200});


        Textbox sampleBox(20, sf::Color::White, false, 4, "Samples: ");
        sampleBox.setFont(arial);
        sampleBox.setPos({0, 250});


        Textbox* selectedBox = nullptr;    
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
                        }else {
                            preview.setColor(sf::Color::Green);
                        }
                        break;
                    case sf::Event::MouseButtonPressed:
                        if(preview.onButton(window)) {
                            if(checkIfPosNum(fovBox.getInput()) && fovBox.getInput() != ""){
                                loadedScene->setFov((M_PI * std::stof(fovBox.getInput()) / 180));
                            }

                            if(checkIfPosNum(focusBox.getInput()) && focusBox.getInput() != ""){
                                loadedScene->setFocusDist(std::stof(focusBox.getInput()));
                            }

                            Renderer previewCreator(500, 400, loadedScene);

                            if(checkIfPosNum(dofBox.getInput()) && dofBox.getInput() != ""){
                                previewCreator.setDof(std::stof(dofBox.getInput()));
                            }
                            createImg(previewCreator.parallelRender(1));
                            saveImage("preview.png");
                            image.loadFromFile("preview.png");
                            texture.loadFromImage(image);  
                            sprite.setTexture(texture);
                            sprite.setPosition(200, 0);
                            }

                        if(fovBox.onButton(window)) {
                            if(selectedBox) {
                                selectedBox->setUnselected();
                                selectedBox->setColor(sf::Color::White);
                            }
                            fovBox.setSelected();
                            fovBox.setColor(sf::Color::Red);
                            selectedBox = &fovBox;
                        }

                        if(resXbox.onButton(window)) {
                            if(selectedBox) {
                                selectedBox->setUnselected();
                                selectedBox->setColor(sf::Color::White);
                            }
                            resXbox.setSelected();
                            resXbox.setColor(sf::Color::Red);
                            selectedBox = &resXbox;
                        }

                        if(resYbox.onButton(window)) {
                            if(selectedBox) {
                                selectedBox->setUnselected();
                                selectedBox->setColor(sf::Color::White);
                            }
                            resYbox.setSelected();
                            resYbox.setColor(sf::Color::Red);
                            selectedBox = &resYbox;
                            
                        }

                        if(dofBox.onButton(window)) {
                            if(selectedBox) {
                                selectedBox->setUnselected();
                                selectedBox->setColor(sf::Color::White);
                            }
                            dofBox.setSelected();
                            dofBox.setColor(sf::Color::Red);
                            selectedBox = &dofBox;
                            
                        }

                        if(sampleBox.onButton(window)) {
                            if(selectedBox) {
                                selectedBox->setUnselected();
                                selectedBox->setColor(sf::Color::White);
                            }
                            sampleBox.setSelected();
                            sampleBox.setColor(sf::Color::Red);
                            selectedBox = &sampleBox;
                        }

                        if(focusBox.onButton(window)) {
                            if(selectedBox) {
                                selectedBox->setUnselected();
                                selectedBox->setColor(sf::Color::White);
                            }
                            focusBox.setSelected();
                            focusBox.setColor(sf::Color::Red);
                            selectedBox = &focusBox;
                            
                        }

                    case sf::Event::TextEntered:
                        fovBox.typedOn(event);
                        resXbox.typedOn(event);
                        resYbox.typedOn(event);
                        dofBox.typedOn(event);
                        sampleBox.typedOn(event);
                        focusBox.typedOn(event);

                    case sf::Event::KeyPressed:
                        if(event.key.code == sf::Keyboard::Enter){
                            int resX;
                            int resY;
                            int sampleSize;
                            float dof = 0;
                            
                            if(checkIfPosNum(fovBox.getInput()) && fovBox.getInput() != ""){
                                loadedScene->setFov((M_PI * std::stof(fovBox.getInput()) / 180));
                            }

                            if(checkIfPosNum(focusBox.getInput()) && focusBox.getInput() != ""){
                                loadedScene->setFocusDist(std::stof(focusBox.getInput()));
                            }

                            if(checkIfPosNum(dofBox.getInput()) && dofBox.getInput() != ""){
                                dof = std::stof(dofBox.getInput());
                            }

                            if(checkIfPosNum(resXbox.getInput()) && resXbox.getInput() != ""){
                                resX = std::stof(resXbox.getInput());
                            }else {
                                break;
                            }

                            if(checkIfPosNum(resYbox.getInput()) && resYbox.getInput() != ""){
                                resY = std::stof(resYbox.getInput());
                            }else {
                                break;
                            }

                            if(checkIfPosNum(sampleBox.getInput()) && sampleBox.getInput() != ""){
                                sampleSize = std::stoi(sampleBox.getInput());
                            }else {
                                break;
                            }
                            window.close();
                            openRender(resX, resY, loadedScene, sampleSize, dof);
                        }
                }
                }

            window.clear();
            preview.draw(window);
            window.draw(sprite);
            fovBox.draw(window);
            resXbox.draw(window);
            resYbox.draw(window);
            focusBox.draw(window);
            dofBox.draw(window);
            sampleBox.draw(window);
            window.display();

            }

        }

    void openRender(int resX, int resY, std::shared_ptr<Scene> loadedScene, int sampleSize, float dof) {
        sf::RenderWindow window(sf::VideoMode(resX, resY), "Path Tracer", sf::Style::Close);
        window.setSize(sf::Vector2u(resX, resY));
        window.setFramerateLimit(0);
        sf::Image image; 
        sf::Sprite sprite;
        sf::Texture texture;
        Renderer sceneRenderer(resX, resY, loadedScene);
        std::vector<std::vector<Color>> combinedSamples;

        int i = 0;
        
            while (window.isOpen())
            {
                sf::Event event;
                while (window.pollEvent(event))
                {   
                    switch(event.type) {
                        case sf::Event::Closed:
                            window.close();
                            }
                    }
                while(i < sampleSize) {
                    auto result = sceneRenderer.parallelRender(1);
                    float weight = 1.0 / (1 + i);
                    if(i == 0) {
                        combinedSamples = result;
                    } else {
                        for (int pixel = 0; pixel < resX * resY; ++pixel)
                        {
                            int x = pixel % resX;
                            int y = pixel / resX;
                            
                            combinedSamples[x][y] = clamp(combinedSamples[x][y] * (1 - weight) + clamp(weight * result[x][y].cwiseSqrt()));
                            
                        }
                    }
                    createImg(combinedSamples);
                    saveImage("image.png");
                    image.loadFromFile("image.png");
                    texture.loadFromImage(image);  
                    sprite.setTexture(texture);
                    window.clear();
                    window.draw(sprite);
                    window.display();
                    i++;


                }
        }
        
        }

private:
    bool checkIfPosNum(std::string text) {
        for(auto i : text) {
            if(!std::isdigit(i)) {
                return false;
            }
        }
        return true;
    }

    Color clamp(Color input) {
        float R = input(0) > 1 ? 1 : input(0);
        float G = input(1) > 1 ? 1 : input(1);
        float B = input(2) > 1 ? 1 : input(2);
        return Color(R, G, B);
    }
};