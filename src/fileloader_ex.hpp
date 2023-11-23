#ifndef FILELOADER_EXCEPTION
#define FILELOADER_EXCEPTION

#include <exception>
#include <iostream>
#include <string>

class FileLoaderException : public std::exception {
    public:
        FileLoaderException(std::string filepath) : filepath_(filepath) {}

        // Pure virtual functions for getting the error message
        virtual const char* what() const noexcept = 0;

        std::string getFilePath() const { return filepath_; }

        virtual ~FileLoaderException() = default;

    private:
        std::string filepath_;

};

class InvalidFilepathException : public FileLoaderException {
    public:
        InvalidFilepathException(std::string filepath) : FileLoaderException(filepath) {
            msg_ = "Exception caught:\nInvalid filepath: " + filepath;
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

class NegativeRadiusException : public FileLoaderException {
    public:
        NegativeRadiusException(std::string filepath, float radius, int line) : FileLoaderException(filepath) {
            msg_ = "Exception caught:\nNegative radius (" + std::to_string(radius) + ") in file: " + filepath + ", on line: " + std::to_string(line);
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

class InvalidSizeVectorException : public FileLoaderException {
    public:
        InvalidSizeVectorException(std::string filepath, size_t size, int line) : FileLoaderException(filepath) {
            msg_ = "Exception caught:\nInvalid vector of size " + std::to_string(size) + " in file: " + filepath + ", on line: " + std::to_string(line);
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

class NegativeFOVException : public FileLoaderException {
    public:
        NegativeFOVException(std::string filepath, float fow, int line) : FileLoaderException(filepath) {
            msg_ = "Exception caught:\nNegative FOW: " + std::to_string(fow) + ", in file: " + filepath + ", on line: " + std::to_string(line);
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

class NegativeFocusException : public FileLoaderException {
    public:
        NegativeFocusException(std::string filepath, float focus, int line) : FileLoaderException(filepath) {
            msg_ = "Exception caught:\nNegative focus distance: " + std::to_string(focus) + " in file: " + filepath + ", on line: " + std::to_string(line);
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

class InvalidKeyException : public FileLoaderException {
    public:
        InvalidKeyException(std::string filepath, std::string key) : FileLoaderException(filepath) {
            msg_ = "Exception caught:\nInvalid key: " + key + " for file: " + filepath;
        }

        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};


#endif