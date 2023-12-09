#ifndef GUI_EXCEPTION
#define GUI_EXCEPTION

#include <exception>
#include <iostream>
#include <string>

class GuiException : public std::exception {
    public:
        GuiException() {}

        virtual const char* what() const noexcept = 0;

        virtual ~GuiException() = default;
};

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