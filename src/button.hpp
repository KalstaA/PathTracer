#pragma once 

#include <iostream>
#include <SFML/Graphics.hpp>
/**
 * @brief Implements a class for creating buttons for a SFML window.
 * 
 */
class Button {                     
    public:

        /**
         * @brief Constructor for the button.
         * 
         * @param button_text The text inside the button
         * @param textColor The color of the text inside the button
         * @param text_size The size of the text inside the button
         * @param button_size Size of the button
         * @param button_color Color of the button
         */
        Button(std::string button_text, sf::Color textColor, int text_size, sf::Vector2f button_size, sf::Color button_color)  {
            button_text_.setString(button_text);
            button_text_.setFillColor(textColor);
            button_text_.setCharacterSize(text_size);

            button_.setFillColor(button_color);
            button_.setSize(button_size);
        }

        /**
         * @brief Destroy the Button object
         * 
         */
        ~Button() = default;


        /**
         * @brief Sets the position of the button in the window
         * 
         * @param pos Position of the button
         */
        void setPos(sf::Vector2f pos) {
            button_.setPosition(pos);

            float text_pos_x = (pos.x + button_.getLocalBounds().width / 2);
            float text_pos_y = (pos.y + button_.getLocalBounds().height / 2);
            button_text_.setPosition({text_pos_x, text_pos_y});
        }

        /**
         * @brief Sets the color of the button
         * 
         * @param color The color to be set to
         */
        void setColor(sf::Color color) {
            button_.setFillColor(color);
        }

        /**
         * @brief Sets the font of the text of the button.
         * 
         * @param font The font to be set to
         */
        void setFont(sf::Font &font) {
            button_text_.setFont(font);
        }

        /**
         * @brief Renders the button on the specified SFML window.
         * 
         * @param window Window to be rendered on
         */
        void draw(sf::RenderWindow &window) {
            window.draw(button_);
            window.draw(button_text_);
        }

        /**
         * @brief Checks if the user's mouse is on the button.
         * 
         * @param window reference to the window the textbox is on
         * @return true value if mouse is on the button
         * @return false value if mouse is not on the button
         */
        bool onButton(sf::RenderWindow &window) {
            float mousePos_x = sf::Mouse::getPosition(window).x;
            float mousePos_y = sf::Mouse::getPosition(window).y;
            float butPos_x = button_.getPosition().x;
            float butPos_y = button_.getPosition().y;
            float butSize_x = button_.getSize().x;
            float butSize_y = button_.getSize().y;

            bool onButton_x = (mousePos_x >= butPos_x) && (mousePos_x <= (butSize_x + butPos_x));
            bool onButton_y = (mousePos_y >= butPos_y) && (mousePos_y <= (butSize_y + butPos_y));

            return (onButton_x && onButton_y);
    
        }


    private: 
    sf::Text button_text_;
    sf::RectangleShape button_;
};