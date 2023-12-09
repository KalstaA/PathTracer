#ifndef FILELOADER_EXCEPTION
#define FILELOADER_EXCEPTION

#include <exception>
#include <iostream>
#include <string>

/**
 * @brief An abstract base class for FileLoader exceptions. Exceptions are designed for invalid user inputs in the YAML files.
 * 
 */
class FileLoaderException : public std::exception {
    public:
        FileLoaderException() {}

        /**
        * @brief Pure virtual function overload for exception messages.
        */
        virtual const char* what() const noexcept = 0;

        virtual ~FileLoaderException() = default;

};

/**
 * @brief Representation of invalid filepath exception.
 * 
 */
class InvalidFilepathException : public FileLoaderException {
    public:
        /**
        * @brief Constructor for InvalidFilePathException.
        * 
        * @param filepath Filepath of the given YAML file
        */
        InvalidFilepathException(std::string filepath) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nInvalid filepath: " + filepath;
        }

        /**
        * @brief Function that creates an exception message for invalid filepath.
        * 
        * @return Pointer to the first char of the exception message.
        */
        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

/**
 * @brief Representation of negative radius exception.
 * 
 */
class NegativeRadiusException : public FileLoaderException {
    public:
        /**
        * @brief Constructor for NegativeRadiusException.
        * 
        * @param filepath Filepath of the YAML file
        * @param radius Value of the negative radius
        * @param line Line number where the negative radius is defined
        */
        NegativeRadiusException(std::string filepath, float radius, int line) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nNegative radius (" +
                    std::to_string(radius) + ") in file: " +
                    filepath + ", on line: " + std::to_string(line);
        }

        /**
        * @brief Function that creates an exception message for negative radius.
        * 
        * @return Pointer to the first char of the exception message.
        */
        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

/**
 * @brief Representation of invalid size vector exception. 
 * 
 */
class InvalidSizeVectorException : public FileLoaderException {
    public:
        /**
        * @brief Constructor for InvalidSizeVectorException.
        * 
        * @param filepath Filepath of the YAML file
        * @param size Size of the invalid size vector
        * @param line Line number where the invalid size vector is defined
        */
        InvalidSizeVectorException(std::string filepath, size_t size, int line) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nInvalid vector of size " +
                    std::to_string(size) + " in file: " + filepath +
                    ", on line: " + std::to_string(line);
        }

        /**
        * @brief Function that creates an exception message for invalid size vector
        * 
        * @return Pointer to the first char of the exception message
        */
        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

/**
 * @brief Representation of negative field of view exception. 
 * 
 */
class NegativeFOVException : public FileLoaderException {
    public:
        /**
        * @brief Constructor for NegativeFOVException.
        * 
        * @param filepath Filepath of the YAML file
        * @param fow Value of the negative field of view
        * @param line Line number where the negative fow is defined
        */
        NegativeFOVException(std::string filepath, float fow, int line) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nNegative FOW: " + std::to_string(fow) +
                    ", in file: " + filepath + ", on line: " + std::to_string(line);
        }

        /**
        * @brief Function that creates an exception message for negative field of view
        * 
        * @return Pointer to the first char of the exception message
        */
        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

/**
 * @brief Representation of negative focus exception. 
 * 
 */
class NegativeFocusException : public FileLoaderException {
    public:
        /**
        * @brief Constructor for NegativeFocusException.
        * 
        * @param filepath Filepath of the YAML file
        * @param focus Value of the negative focus distance
        * @param line Line number where the negative dimension is defined
        */
        NegativeFocusException(std::string filepath, float focus, int line) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nNegative focus distance: " +
                    std::to_string(focus) + " in file: " + filepath +
                    ", on line: " + std::to_string(line);
        }

        /**
        * @brief Function that creates an exception message for negative focus distance
        * 
        * @return Pointer to the first char of the exception message
        */
        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

/**
 * @brief Representation of invalid key exception. 
 * 
 */
class InvalidKeyException : public FileLoaderException {
    public:
        /**
        * @brief Constructor for InvalidKeyException.
        * 
        * @param filepath Filepath of the YAML file
        * @param key String of the key that was not found from the YAML file
        */
        InvalidKeyException(std::string filepath, std::string key) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nInvalid key: " + key + ", for file: " + filepath;
        }

        /**
        * @brief Function that creates an exception message for invalid key value in the YAML file
        * 
        * @return Pointer to the first char of the exception message
        */
        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

/**
 * @brief Representation of material not found exception. 
 * 
 */
class MaterialNotFoundException : public FileLoaderException {
    public:
        /**
        * @brief Constructor for MaterialNotFoundException.
        * 
        * @param filepath Filepath of the YAML file
        * @param line Line number of the object that do not have material defined
        */
        MaterialNotFoundException(std::string filepath, int line) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nMaterial not defined for object at line: " +
                    std::to_string(line) + ", in file: " + filepath + ".";
        }

        /**
        * @brief Function that creates an exception message for objects that are missing the material definition in the YAML file
        * 
        * @return Pointer to the first char of the exception message
        */
        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;
};

/**
 * @brief Representation of radius not found exception. 
 * 
 */
class RadiusNotFoundException : public FileLoaderException {
    public:
        /**
        * @brief Constructor for RadiusNotFoundException.
        * 
        * @param filepath Filepath of the YAML file
        * @param line Line number of the ball that do not have radius defined
        */
        RadiusNotFoundException(std::string filepath, int line) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nRadius not defined for ball at line: " +
                    std::to_string(line) + ", in file: " + filepath + ".";
        }

        /**
        * @brief Function that creates an exception message for ball object that do not have radius defined in the YAML file
        * 
        * @return Pointer to the first char of the exception message
        */
        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;
};

/**
 * @brief Representation of parameter not found exception.
 * 
 */
class ParameterNotFoundException : public FileLoaderException {
    public:
        /**
        * @brief Constructor for ParameterNotFoundException.
        * 
        * @param filepath Filepath of the YAML file
        * @param line Line number where the parameter is missing
        */
        ParameterNotFoundException(std::string filepath, int line) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nMissing parameters in line: " +
                    std::to_string(line) + ", in file: " + filepath + ".";
        }

        /**
        * @brief Function that creates an exception message for missing parameters
        * 
        * @return Pointer to the first char of the exception message
        */
        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;
};

/**
 * @brief Representation of negative dimension exception. 
 * 
 */
class NegativeDimensionException : public FileLoaderException {
    public:
        /**
        * @brief Constructor for NegativeDimensionException.
        * 
        * @param filepath Filepath of the YAML file
        * @param value Value of the negative dimension
        * @param line Line number where the negative dimension is defined
        */
        NegativeDimensionException(std::string filepath, float value, int line) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nNegative dimension: " +
                    std::to_string(value) + " in file: " + filepath +
                    ", on line: " + std::to_string(line);
        }

        /**
        * @brief Function that creates an exception message for negative dimension value
        * 
        * @return Pointer to the first char of the exception message
        */
        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;

};

/**
 * @brief Representation of invalid material type exception. 
 * 
 */
class InvalidMaterialTypeException : public FileLoaderException {
    public:
        /**
        * @brief Constructor for InvalidMaterialTypeException.
        * 
        * @param filepath Filepath of the YAML file
        * @param line Line number where material is defined
        */
        InvalidMaterialTypeException(std::string filepath, int line) : FileLoaderException() {
            msg_ = "FileLoader exception caught:\nInvalid material type in file: " +
                    filepath + ", for material starting on line: " + std::to_string(line) + ".";
        }

        /**
        * @brief Function that creates an exception message for invalid material type in the YAML file
        * 
        * @return Pointer to the first char of the exception message
        */
        virtual const char* what() const noexcept {
            return msg_.c_str();
        }

    private:
        std::string msg_;
};


#endif