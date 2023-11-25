#ifndef FILELOADER_EXCEPTION
#define FILELOADER_EXCEPTION

#include <exception>
#include <iostream>
#include <string>

class FileLoaderException : public std::exception {
    public:
        FileLoaderException() {}

        // Pure virtual functions for getting the error message
        virtual const char* what() const noexcept = 0;

        virtual ~FileLoaderException() = default;

};

class InvalidFilepathException : public FileLoaderException {
    public:
        InvalidFilepathException(std::string filepath) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nInvalid filepath: " + filepath;
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

class NegativeRadiusException : public FileLoaderException {
    public:
        NegativeRadiusException(std::string filepath, float radius, int line) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nNegative radius (" +
                    std::to_string(radius) + ") in file: " +
                    filepath + ", on line: " + std::to_string(line);
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

class InvalidSizeVectorException : public FileLoaderException {
    public:
        InvalidSizeVectorException(std::string filepath, size_t size, int line) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nInvalid vector of size " +
                    std::to_string(size) + " in file: " + filepath +
                    ", on line: " + std::to_string(line);
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

class NegativeFOVException : public FileLoaderException {
    public:
        NegativeFOVException(std::string filepath, float fow, int line) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nNegative FOW: " + std::to_string(fow) +
                    ", in file: " + filepath + ", on line: " + std::to_string(line);
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

class NegativeFocusException : public FileLoaderException {
    public:
        NegativeFocusException(std::string filepath, float focus, int line) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nNegative focus distance: " +
                    std::to_string(focus) + " in file: " + filepath +
                    ", on line: " + std::to_string(line);
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

class InvalidKeyException : public FileLoaderException {
    public:
        InvalidKeyException(std::string filepath, std::string key) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nInvalid key: " + key + ", for file: " + filepath;
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

class MaterialNotFoundException : public FileLoaderException {
    public:
        MaterialNotFoundException(std::string filepath, int line) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nMaterial not defined for object at line: " +
                    std::to_string(line) + ", in file: " + filepath + ".";
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;
};

class RadiusNotFoundException : public FileLoaderException {
    public:
        RadiusNotFoundException(std::string filepath, int line) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nRadius not defined for ball at line: " +
                    std::to_string(line) + ", in file: " + filepath + ".";
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;
};

/* Need eny more exceptions?:
Color: Invalid color parameters, InvalidColorException
*/


#endif