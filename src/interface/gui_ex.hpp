#ifndef GUI_EXCEPTION
#define GUI_EXCEPTION

#include <exception>
#include <iostream>
#include <string>

/**
 * @brief Abstact class for a GUI exception
 * 
 */
class GuiException : public std::exception {
    public:
        GuiException() {}

        virtual const char* what() const noexcept = 0;

        virtual ~GuiException() = default;
};

/**
 * @brief Exception for unexpected behaviour in the title screen
 * 
 */
class TitleScreenException : public GuiException {
    public:
        TitleScreenException() : GuiException() {
            msg_ = "Title window closed";
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;
};

/**
 * @brief Exception for not finding the given font
 * 
 */
class FontException : public GuiException {
    public:
        FontException(std::string fontPath) : GuiException() {
            msg_ = "Did not find font in path: " + fontPath;
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;
};

#endif