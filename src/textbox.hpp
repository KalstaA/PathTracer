#pragma once 

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#define BACKSPACE_KEY 8
#define ESCAPE_KEY 27

class Textbox {
public:
    Textbox() {}

    Textbox(int size, sf::Color color, bool isSelected, int limit, std::string preText) {
        textbox_.setCharacterSize(size);
        size_ = size;
        textbox_.setFillColor(color);
        textbox_.setString(preText);
        isSelected_ = isSelected;
        limit_ = limit + preText.length() - 1;
        preTextLength = preText.length();
        text_ << preText;
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

    void setColor(sf::Color textColor) {
        textbox_.setFillColor(textColor);
    }

    void draw(sf::RenderWindow &window) {
        window.draw(textbox_);
    }

    bool onButton(sf::RenderWindow &window) {
            float mousePos_x = sf::Mouse::getPosition(window).x;
            float mousePos_y = sf::Mouse::getPosition(window).y;
            float boxPos_x = textbox_.getPosition().x;
            float boxPos_y = textbox_.getPosition().y;
            float boxSize_x = (size_/1.618)*limit_;
            float boxSize_y = size_;

            bool onBox_x = (mousePos_x >= boxPos_x) && (mousePos_x <= (boxSize_x + boxPos_x));
            bool onBox_y = (mousePos_y >= boxPos_y) && (mousePos_y <= (boxSize_y + boxPos_y));

            return (onBox_x && onBox_y);
    
        }

    std::string getInput() {
            return text_.str().substr(preTextLength);
        }

    void typedOn(sf::Event input) {
        if(isSelected_) {
            int inputChar = input.text.unicode;
            if(inputChar < 128 && inputChar != 0) {
                if(text_.str().length() <= limit_) {
                    checkInput(inputChar);
                }else if(inputChar == BACKSPACE_KEY) {
                    deleteLastChar();
                }
            }
        }
    }

private:
    sf::Text textbox_;
    std::ostringstream text_;
    bool isSelected_ = false;
    int limit_;
    int preTextLength;
    int size_;

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
        textbox_.setString(text_.str());
    }

    void deleteLastChar() {
        if(text_.str().length() > preTextLength){
            std::string newTxt = text_.str();
            newTxt.pop_back();
            text_.str(std::string());
            text_ << newTxt;
            textbox_.setString(text_.str());
        }
    }

};