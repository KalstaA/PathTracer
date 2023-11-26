#pragma once 

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#define BACKSPACE_KEY 8
#define ESCAPE_KEY 27

class Textbox {
public:
    Textbox() {}

    Textbox(int size, sf::Color color, bool isSelected, int limit) {
        textbox_.setCharacterSize(size);
        textbox_.setFillColor(color);
        isSelected_ = isSelected;
        limit_ = limit;
    }

    void setFont(sf::Font &font) {
        textbox_.setFont(font);
    }

    void setSelected() {
        isSelected_ = true;
    }

    void setUnselected() {
        isSelected_ = false;
    }

    void setPos(sf::Vector2f pos) {
        textbox_.setPosition(pos);
    }

    void draw(sf::RenderWindow &window) {
        window.draw(textbox_);
    }

    std::string getInput() {
            return text_.str();
        }

    void typedOn(sf::Event input) {
        if(isSelected_) {
            int inputChar = input.text.unicode;
            if(inputChar < 128) {
                if(text_.str().length() <= limit_) {
                    checkInput(inputChar);
                }
            }
        }
    }

private:
    sf::Text textbox_;
    std::ostringstream text_;
    bool isSelected_ = false;
    int limit_;

    void checkInput(int inputChar) {
        if(inputChar != BACKSPACE_KEY && inputChar != ESCAPE_KEY) {
            text_ << static_cast<char>(inputChar);
        }
        else if(inputChar == BACKSPACE_KEY) {
            deleteLastChar();
        } 
        else if(inputChar == ESCAPE_KEY && isSelected_ == true) {
            setUnselected();
        }
    }

    void deleteLastChar() {
        std::string newTxt = text_.str();
        newTxt.pop_back();
        text_ << newTxt;
        textbox_.setString(text_.str());
    }

};