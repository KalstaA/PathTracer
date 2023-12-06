#pragma once 

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#define BACKSPACE_KEY 8
#define ESCAPE_KEY 27
#define TAB_KEY 9

/**
 * @brief Implements a class for creating textboxes that take inputs into SFML-window.
 * 
 */
class Textbox {
public:

    /**
     * @brief Construct a new Textbox object.
     * 
     * @param size Size of the text
     * @param color Color of the text
     * @param isSelected A boolean value telling if the box is selected
     * @param limit Limit to the the user can input into the box
     * @param preText A non modifiable text in the box before the input space
     */
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

    /**
     * @brief Destroy the Textbox object.
     * 
     */
    ~Textbox() = default;

    /**
     * @brief Set the font of the texbox text.
     * 
     * @param font a loaded font for the text
     */
    void setFont(sf::Font &font) {
        textbox_.setFont(font);
    }

    /**
     * @brief Set the object to be selected.
     * 
     */
    void setSelected() {
        isSelected_ = true;
    }

    /**
     * @brief Set the object to be unselected.
     * 
     */
    void setUnselected() {
        isSelected_ = false;
    }

    /**
     * @brief Sets the position of the box in the SFML window.
     * 
     * @param pos Position of the box
     */
    void setPos(sf::Vector2f pos) {
        textbox_.setPosition(pos);
    }

    /**
     * @brief Set the color of the text inside the box.
     * 
     * @param textColor Color that the text is to be set to
     */
    void setColor(sf::Color textColor) {
        textbox_.setFillColor(textColor);
    }

    /**
     * @brief Renders a box on a specified window.
     * 
     * @param window a reference to the window
     */
    void draw(sf::RenderWindow &window) {
        window.draw(textbox_);
    }

    /**
     * @brief Checks if the users mouse is on the textbox.
     * 
     * @param window reference to the window the textbox is on
     * @return true value if mouse is on the box 
     * @return false value if mouse is not on the box
     */
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

    /**
     * @brief Get the text typed into the textbox.
     * 
     * @return std::string of the input text
     */
    std::string getInput() {
            return text_.str().substr(preTextLength);
        }

    /**
     * @brief Takes an event, that is supposed to be a keypress, and acts accrodingly: checks if the box is selected
     * and if it is the input is checked. If the input is semi-logical (unicode < 128) and the character limit
     * is not exceeded, the character is added to the box. Backspace key deletes the previous inputted character
     * instead of adding it to the box.
     * 
     * 
     * @param input a sf::Event of what the user is doing eg. clicking a button or moving mouse
     */
    void typedOn(sf::Event input) {
        if(isSelected_) {
            int inputChar = input.text.unicode;
            if(inputChar < 128 && inputChar != 0 && inputChar != TAB_KEY) {
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

    /**
     * @brief Checks the inputted character and acts accordingly. The character is added to the textboxes text if 
     * it is not a backspace- or  an escape-key. If it is a backspace the deleteLastChar() function is called. If escape the 
     * input, the box is unselected.
     *
     * 
     * @param inputChar unicode number of the inputted character
     */
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

    /**
     * @brief Deletes the last character of the boxe's text if there is user inputted text in the box.
     * 
     */
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