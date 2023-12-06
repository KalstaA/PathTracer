#pragma once 

#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include "button.hpp"
#include "textbox.hpp"
#include "fileloader.hpp"
#include "types.hpp"
#include "gui_ex.hpp"

/**
 * @brief Implements the Graphical user interface class.
 * 
 */
class Gui : public Interface {
public:

    /**
     * @brief Opens a titlescreen where the user can input a filepath. If the filepath contains a valid yaml file, 
     * a shared pointer to a scene constructed with this file is returned. An invalid filepath will result in an error
     * being shown to the user.
     * 
     * @return std::shared_ptr<Scene> The shared pointer to the scene created with the inputted yaml file
     */
    std::shared_ptr<Scene> titleScreen() {
        sf::Font arial;   
        std::string arialpath = "../src/Arial.ttf";
        if(!arial.loadFromFile(arialpath)) {
            throw FontException(arialpath);
        }
        sf::Font comic;
        std::string comicpath = "../src/ComicSansMS.ttf";
        if(!comic.loadFromFile(comicpath)) {
            throw FontException(comicpath);
        }
        
        selectedBox = nullptr;

        sf::RenderWindow window(sf::VideoMode(700, 400), "Path Tracer", sf::Style::Titlebar | sf::Style::Close);

        //Creates all UI elements
        sf::Text title;
        title.setFillColor(sf::Color::White);
        title.setFont(comic);
        title.setCharacterSize(50);
        title.setString("Path Tracer");
        title.setPosition({175, 150});

        sf::Text invalidPath; 
        invalidPath.setFillColor(sf::Color::Red);
        invalidPath.setFont(arial);
        invalidPath.setCharacterSize(25);
        invalidPath.setString("Invalid filepath");
        invalidPath.setPosition({175, 250});

        bool error = false;

        Textbox filepathBox(25, sf::Color::White, false, 30, "Enter filename: ");
        filepathBox.setFont(arial);
        filepathBox.setPos({175, 225});

        while(window.isOpen()) {
            sf::Event event;
            //Checks user created events constantly
            while(window.pollEvent(event)) {
                switch(event.type) {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    case sf::Event::MouseButtonPressed:
                        if(filepathBox.onButton(window)) {
                            filepathBox.setSelected();
                            filepathBox.setColor(sf::Color::Green);
                        }
                        break;
                    case sf::Event::TextEntered:
                        filepathBox.typedOn(event);
                        break;
                    case sf::Event::KeyPressed:
                        if(event.key.code == sf::Keyboard::Enter){ 
                            try 
                            {
                                std::string filepath = filepathBox.getInput().substr();
                                FileLoader loader(filepath);
                                window.close();
                                std::shared_ptr<Scene> loadedScene = loader.loadSceneFile();
                                return loadedScene;
                            } 
                            catch(FileLoaderException& ex) {
                                error = true;
                            }
                        } else if(event.key.code == sf::Keyboard::Escape){
                            filepathBox.setColor(sf::Color::White);
                        }
                        break;
                    default: 
                        break;
                }
            } 

            window.clear();
            window.draw(title);
            if(error) {
                window.draw(invalidPath);
            }
            filepathBox.draw(window);
            window.display();

        }
        throw TitleScreenException();
    }


    /**
     * @brief Opens a settings menu where the user can preview and influence a loaded scene. 
     *  
     * The setting menu contains a "preview" button and textboxes for parameter. There are two 
     * types of parameters that the user can change. First are the parameters that influence only the final
     * render the user can initiate with pressing enter. The other parameters, in addition to changing the final 
     * render, affect the preview image.
     *  
     * 
     * @param loadedScene the scene to be influenced 
     */
    void openSettings(std::shared_ptr<Scene> loadedScene) {
        sf::Image image; 
        sf::Sprite sprite;
        sf::Texture texture;
        sf::Font arial;
        std::string arialpath = "../src/Arial.ttf";
        if(!arial.loadFromFile(arialpath)) {
            throw FontException(arialpath);
        }

        sf::RenderWindow window(sf::VideoMode(700, 400), "Path Tracer", sf::Style::Titlebar | sf::Style::Close);
        
        //Creates all the UI elements
        sf::Vector2f size(200, 100);
        Button preview("Preview", sf::Color::Black, 20, size, sf::Color::Green);
        preview.setFont(arial);
        preview.setPos({0, 300});

        Textbox resXbox(20, sf::Color::White, false, 4, "ResX: ");
        resXbox.setFont(arial);
        resXbox.setPos({0, 0});

        Textbox resYbox(20, sf::Color::White, false, 4, "ResY: ");
        resYbox.setFont(arial);
        resYbox.setPos({0, 25});

        Textbox sampleBox(20, sf::Color::White, false, 4, "Samples: ");
        sampleBox.setFont(arial);
        sampleBox.setPos({0, 50});

        Textbox bounceBox(20, sf::Color::White, false, 4, "Light bounces: ");
        bounceBox.setFont(arial);
        bounceBox.setPos({0, 75});

        Textbox fovBox(20, sf::Color::White, false, 4, "Fov: ");
        fovBox.setFont(arial);
        fovBox.setPos({0, 125});

        Textbox dofBox(20, sf::Color::White, false, 4, "Depth of field: ");
        dofBox.setFont(arial);
        dofBox.setPos({0, 150});

        Textbox focusBox(20, sf::Color::White, false, 4, "Focus distance: ");
        focusBox.setFont(arial);
        focusBox.setPos({0, 175});

        sf::Text errorText;
        errorText.setFont(arial);
        errorText.setFillColor(sf::Color::Red);
        errorText.setCharacterSize(20);
        errorText.setPosition({0, 225});
  
        selectedBox = nullptr;

        while (window.isOpen())
        {
            sf::Event event;
            //Checks user created events constantly
            while (window.pollEvent(event))
            {   
                switch(event.type) {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    case sf::Event::MouseMoved:
                        if(preview.onButton(window)) {
                            preview.setColor(sf::Color::White);
                        }else {
                            preview.setColor(sf::Color::Green);
                        }
                        break;
                    case sf::Event::MouseButtonPressed:
                        //Checks if preview button can be clicked
                        if(preview.onButton(window)) {
                            if(checkIfPosFloat(fovBox.getInput()) && fovBox.getInput() != ""){
                                loadedScene->setFov((M_PI * std::stof(fovBox.getInput()) / 180));
                            }

                            if(checkIfPosFloat(focusBox.getInput()) && focusBox.getInput() != ""){
                                loadedScene->setFocusDist(std::stof(focusBox.getInput()));
                            }

                            Renderer previewCreator(500, 400, loadedScene);

                            if(checkIfPosFloat(dofBox.getInput()) && dofBox.getInput() != ""){
                                previewCreator.setDof(std::stof(dofBox.getInput()));
                            }
                            createImg(previewCreator.parallelRender(1));
                            saveImage("preview.png");
                            image.loadFromFile("preview.png");
                            texture.loadFromImage(image);  
                            sprite.setTexture(texture);
                            sprite.setPosition(200, 0);
                            }
                        //checks if any textboxes can be clicked
                        clickBox(resXbox, window);
                        clickBox(resYbox, window);
                        clickBox(sampleBox, window);
                        clickBox(bounceBox, window);
                        clickBox(fovBox, window);
                        clickBox(dofBox, window);
                        clickBox(focusBox, window);
                        break;

                    case sf::Event::TextEntered:
                        fovBox.typedOn(event);
                        resXbox.typedOn(event);
                        resYbox.typedOn(event);
                        dofBox.typedOn(event);
                        sampleBox.typedOn(event);
                        focusBox.typedOn(event);
                        bounceBox.typedOn(event);
                        break;

                    case sf::Event::KeyPressed:
                        if(event.key.code == sf::Keyboard::Enter){
                            int resX;
                            int resY;
                            int sampleSize;
                            int bounceAmount;
                            float dof = 0;
                            
                            if(checkIfPosFloat(fovBox.getInput()) && fovBox.getInput() != ""){
                                loadedScene->setFov((M_PI * std::stof(fovBox.getInput()) / 180));
                            }

                            if(checkIfPosFloat(focusBox.getInput()) && focusBox.getInput() != ""){
                                loadedScene->setFocusDist(std::stof(focusBox.getInput()));
                            }

                            if(checkIfPosFloat(dofBox.getInput()) && dofBox.getInput() != ""){
                                dof = std::stof(dofBox.getInput());
                            }

                            if(checkIfPosNum(resXbox.getInput()) && resXbox.getInput() != ""){
                                resX = std::stof(resXbox.getInput());
                            }else {
                                errorText.setString("Invalid resX");
                                break;
                            }

                            if(checkIfPosNum(resYbox.getInput()) && resYbox.getInput() != ""){
                                resY = std::stof(resYbox.getInput());
                            }else {
                                errorText.setString("Invalid resY");
                                break;
                            }

                            if(checkIfPosNum(sampleBox.getInput()) && sampleBox.getInput() != ""){
                                sampleSize = std::stoi(sampleBox.getInput());
                            }else {
                                errorText.setString("Invalid sample amount");
                                break;
                            }

                            if(checkIfPosNum(bounceBox.getInput()) && bounceBox.getInput() != ""){
                                bounceAmount = std::stoi(bounceBox.getInput());
                            }else {
                                errorText.setString("Invalid bounce amount");
                                break;
                            }
                            
                            selectedBox = nullptr;
                            window.close();
                            openRender(resX, resY, loadedScene, sampleSize, dof, bounceAmount);
                            break;  
                        }
                    default:
                        break;

                    }
                }

            window.clear();
            preview.draw(window);
            window.draw(sprite);
            window.draw(errorText);
            fovBox.draw(window);
            resXbox.draw(window);
            resYbox.draw(window);
            focusBox.draw(window);
            dofBox.draw(window);
            sampleBox.draw(window);
            bounceBox.draw(window);
            window.display();

            }

        }

    void openRender(int resX, int resY, std::shared_ptr<Scene> loadedScene, int sampleSize, float dof, int bounces) {
        sf::RenderWindow window(sf::VideoMode(resX, resY), "Path Tracer", sf::Style::Close);
        window.setSize(sf::Vector2u(resX, resY));
        window.setFramerateLimit(0);
        sf::Image image; 
        sf::Sprite sprite;
        sf::Texture texture;
        Renderer sceneRenderer(resX, resY, loadedScene);
        sceneRenderer.setMaxBounces(bounces);
        sceneRenderer.setDof(dof);
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
                            break;
                        default:
                            break;
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
    Textbox* selectedBox = nullptr;  

    /**
     * @brief Checks if the given string is a positive integer.
     * 
     * @param text string to be checked
     * @return true if the string is a positive integer
     * @return false if the string is not a positive integer
     */
    bool checkIfPosNum(std::string text) {
        for(auto i : text) {
            if(!std::isdigit(i)) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Checks if the given string is a positive float with at most one decimal.
     * 
     * @param text string to be checked
     * @return true if the string is a positive float 
     * @return false if the string is not a positive float 
     */
    bool checkIfPosFloat(std::string text) {
        int j = text.length();
        for(auto i : text) {
            if(!std::isdigit(i)) {
                if(j != 2 && std::to_string(text[text.length()-2]) != ".") {
                    return false;
                }
            }
            j--;
        }
        return true;
    }

    /**
     * @brief A helper function to handle what happens when mouse is clicked.
     * 
     * Checks if mouse is on the specified textbox. If it is, the previously selected box is unselected
     * and box is set as the selected box. 
     * 
     * @param box textbox to be checked
     * @param window the window that contains the textbox
     */
    void clickBox(Textbox &box, sf::RenderWindow &window) {
        if(box.onButton(window)) {
             if(selectedBox) {
                selectedBox->setUnselected();
                selectedBox->setColor(sf::Color::White);
                }
                box.setSelected();
                box.setColor(sf::Color::Green);
                selectedBox = &box;    
                }
    }

    /**
     * @brief Clamps the given color values, i.e., ensures that the maximum value for each R, G, B is 1.
     * 
     * @param input the color vector to be clamped
     * @return Color with the values clamped
     */
    Color clamp(Color input) {
        float R = input(0) > 1 ? 1 : input(0);
        float G = input(1) > 1 ? 1 : input(1);
        float B = input(2) > 1 ? 1 : input(2);
        return Color(R, G, B);
    }
};