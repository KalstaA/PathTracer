#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "types.hpp"

class Interface {
private:
    sf::Image img;

    /**
     * @brief Scale the input from [0, 1] to [0, 255]
     * 
     * @param x number to be scaled
     */
    sf::Uint8 scale(float x) {
        return floor(255*x);
    }

public:

    /**
     * @brief Create a sf::Image from the matrix of RGB values
     * 
     * @param pixels matrix of RGB values
     */
    void createImg(std::vector<std::vector<Color>> pixels) {
        int width = pixels.size(); 
        int height = pixels[0].size();
        img.create(width, height);
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                const sf::Uint8 R = scale(pixels[i][j](0));
                const sf::Uint8 G = scale(pixels[i][j](1));
                const sf::Uint8 B = scale(pixels[i][j](2));
                img.setPixel(i, j, sf::Color(R, G, B));
            }
        }
    }

    /**
     * @brief Save the image with given filename
     * 
     * @param filename Image filename
     */
    bool saveImage(const std::string &filename) {
        return img.saveToFile(filename);
    }

    sf::Image getImage() { return img; }

};
