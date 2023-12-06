#pragma once

#include <yaml-cpp/yaml.h>
#include <string>
#include <exception>
#include <memory>
#include <sys/stat.h>

#include "trianglemesh.hpp"
#include "box.hpp"
#include "rectangle.hpp"
#include "ball.hpp"
#include "scene.hpp"
#include "types.hpp"
#include "fileloader_ex.hpp"
#include "material.hpp"

/**
 * @brief Implements a class for reading yaml scene files.
 * 
 */

class FileLoader {
    public:

        /**
         * @brief Construct a new FileLoader object.
         * 
         * @param filepath Filepath to the yaml file that contains properties of the scene
         */

        FileLoader(std::string filepath) {
            // Check the existence of the given file
            struct stat buf;
            if (stat(filepath.c_str(), &buf) == 0)
            {
                filepath_ = filepath;
            }
            else
            {
                throw InvalidFilepathException(filepath);
            }
        }

        /**
         * @brief Loads a scene from the file specified in filepath and returns a shared pointer to this scene.
         * 
         * @return A shared pointer to scene object
         */
        std::shared_ptr<Scene> loadSceneFile() {
            Camera camera = LoadCamera();
            std::list<std::shared_ptr<Object>> objects = LoadObjects();
            scene_ = std::make_shared<Scene>(camera, objects);
            LoadEnvironment(scene_);
            return scene_;
        }

        /**
         * @brief Destructor for FileLoader object
         */
        ~FileLoader() {}

        // Getter for filepath
        std::string getFilepath() { return filepath_; }


    private:

        /**
         * @brief Takes a pointer to a scene object as an input and sets the environment of the scene to values specified in
         * scene file. If scene file has no "Environment" node, the environment is set to default values.
         * 
         * @param scene A  reference to a scene object
         */
        void LoadEnvironment(std::shared_ptr<Scene> scene) {
            if(YAML::LoadFile(filepath_)["Environment"]) {
                YAML::Node environment_node = loadParams("Environment");
                Eigen::Vector3d skyColor = LoadVector(environment_node, "SkyColor");
                Eigen::Vector3d horizonColor = LoadVector(environment_node, "HorizonColor");
                Eigen::Vector3d groundColor = LoadVector(environment_node, "GroundColor"); 
                (*scene).getEnvironment().setSky(skyColor, horizonColor, groundColor);
            }else {
                (*scene).getEnvironment().setSky();
            }
        }

        /**
         * @brief A helper function to create vectors from yaml sequences.
         * 
         * @param coords An yaml node that points to a key that has a sequence as a value 
         * @return A vector based on values in node
         */
        Eigen::Vector3d LoadVector(YAML::Node node, std::string key) { 
            Eigen::Vector3d vector;
            YAML::Node coords = node[key];
            // Throw exception if incorrect size or if not defined
            if (!coords.IsDefined()) {
                throw InvalidKeyException(filepath_, key);
            }
            size_t size = coords.size();
            if (size != 3) {
                throw InvalidSizeVectorException(filepath_, size, coords.Mark().line);
            }
            int i = 0;
            for (YAML::const_iterator it=coords.begin(); it!=coords.end(); ++it) {
                vector[i] = it->as<float>();
                i++;
            }
            return vector;
        }

        /**
         * @brief Creates a material object based on properties given in yaml node.
         * 
         * @param material_node An yaml node that points to a key that has possible key-value pairs containing
         * material values 
         * @return A shared pointer to material object
         */
        std::shared_ptr<Material> LoadMaterial(YAML::Node node) {
            //Material material
            YAML::Node material_node = node["Material"];
            if (!material_node.IsDefined()) {
                throw MaterialNotFoundException(filepath_, node.Mark().line);
            }

            // Initialize default values
            Color color = Color(1.0, 1.0, 1.0);
            Color emission_color = Color(1.0, 1.0, 1.0);
            float emission_strength = 0.0;
            std::string name = "UNDEFINED";
            float specularity = 1.0;
            float clearcoat = 0.5;
            float refraction_ratio = 1.0;
            Color clearCoat_color = Color(1.0, 1.0, 1.0);

            // Update the parameters based on .yaml file if the parameters are defined
            if(material_node["Color"]) {
                color = LoadVector(material_node, "Color");
            }
            if(material_node["EmissionColor"]) {
                emission_color = LoadVector(material_node, "EmissionColor");
            }
            if(material_node["EmissionStrength"]) {
                emission_strength = material_node["EmissionStrength"].as<float>();
            }
            if(material_node["Specularity"]) {
                specularity = material_node["Specularity"].as<float>();
            }
            if(material_node["Name"]) {
                name = material_node["Name"].as<std::string>();
            }
            if(material_node["ClearCoat"]) {
                clearcoat = material_node["ClearCoat"].as<float>();
            }
            if(material_node["ClearCoatColor"]) {
                clearCoat_color = LoadVector(material_node, "ClearCoatColor");
            }
            if(material_node["RefractionRatio"]) {
                refraction_ratio = material_node["RefractionRatio"].as<float>();
            }

            // Return shared pointer to the correct material type or throw exception
            std::string type = material_node["Type"].as<std::string>();
            if (type == "Diffuse") {
                return std::make_shared<Diffuse>(color, name, emission_strength, emission_color);
            }
            else if (type == "Reflective") {
                return std::make_shared<Reflective>(color, name, specularity);
            }
            else if (type == "ClearCoat") {
                return std::make_shared<ClearCoat>(color, name, specularity, clearcoat, clearCoat_color);
            }
            else if (type == "Refractive") {
                return std::make_shared<Refractive>(color, name, refraction_ratio);
            }
            else {
                throw InvalidMaterialTypeException(filepath_, material_node.Mark().line);
            }
        }

        /**
         * @brief Creates a pointer to a ball object from yaml node.
         * 
         * @param ball Yaml node that contains properties of the ball object
         * @return A pointer to a ball object 
         */
        std::shared_ptr<Ball> LoadBall(YAML::Node ball) {
            YAML::Node radius_node = ball["Radius"];
            if (!radius_node.IsDefined()) {
                throw RadiusNotFoundException(filepath_, ball.Mark().line);
            }
            float radius = radius_node.as<float>();
            if (radius < 0)
            {
                throw NegativeRadiusException(filepath_, radius, radius_node.Mark().line);
            }
            else
            {
                std::shared_ptr<Ball> ball_ptr = std::make_shared<Ball>(LoadVector(ball, "Position"), radius, LoadMaterial(ball));
                return ball_ptr;
            }
        }

        /**
         * @brief Creates a pointer to a box object from yaml node.
         * 
         * @param box Yaml node that contains properties of box object
         * @return A pointer to a box object 
         */
        std::shared_ptr<Box> LoadBox(YAML::Node box) {
            YAML::Node width_node = box["Width"];
            YAML::Node height_node = box["Height"];
            YAML::Node depth_node = box["Depth"];

            if (!width_node.IsDefined() || !height_node.IsDefined() || !depth_node.IsDefined()) {
                throw ParameterNotFoundException(filepath_, box.Mark().line);
            }
            
            float width = width_node.as<float>();
            float height = height_node.as<float>();
            float depth = depth_node.as<float>();

            if (width < 0)
            {
                throw NegativeDimensionException(filepath_, width, width_node.Mark().line);
            }

            if (height < 0)
            {
                throw NegativeDimensionException(filepath_, height, height_node.Mark().line);
            }

            if (depth < 0)
            {
                throw NegativeDimensionException(filepath_, depth, depth_node.Mark().line);
            }
            
            std::shared_ptr<Box> box_ptr = std::make_shared<Box>(LoadVector(box, "Position"), width, height, depth, LoadMaterial(box));
            
            Vector rotation;
            try
            {
                rotation = LoadVector(box, "Rotation");
            }
            catch(const InvalidKeyException& e)
            {
                std::cout << e.what() << std::endl;
                std::cout << "Standard rotation (0, 0, 0) will be used." << std::endl;
                rotation = Vector(0, 0, 0);
            }
            rotation = (rotation / 180.0) * M_PI;

            box_ptr->rotate(rotation[0], Vector::UnitX());
            box_ptr->rotate(rotation[1], Vector::UnitY());
            box_ptr->rotate(rotation[2], Vector::UnitZ());
            
            return box_ptr;
        }

        /**
         * @brief Creates a pointer to a trianglemesh object from yaml node.
         * 
         * @param tmesh Yaml node that contains properties of TriangleMesh object
         * @return A pointer to a TriangleMesh object
         */
        std::shared_ptr<TriangleMesh> LoadTriangleMesh(YAML::Node tmesh) {
            YAML::Node filepath_node = tmesh["Filepath"];
            YAML::Node scale_node = tmesh["Scale"];

            if (!filepath_node.IsDefined() || !scale_node.IsDefined()) {
                throw ParameterNotFoundException(filepath_, tmesh.Mark().line);
            }
            
            std::string tmesh_filepath = filepath_node.as<std::string>();

            struct stat buf;
            if (stat(tmesh_filepath.c_str(), &buf) != 0)
            {
                throw InvalidFilepathException(tmesh_filepath);
            }

            double scale = scale_node.as<double>();

            if (scale < 0)
            {
                throw NegativeDimensionException(filepath_, scale, scale_node.Mark().line);
            }

            Vector rotation;
            try
            {
                rotation = LoadVector(tmesh, "Rotation");
            }
            catch(const InvalidKeyException& e)
            {
                std::cout << e.what() << std::endl;
                std::cout << "Standard rotation (0, 0, 0) will be used." << std::endl;
                rotation = Vector(0, 0, 0);
            }
            rotation = (rotation / 180.0) * M_PI;
            
            std::shared_ptr<TriangleMesh> tmesh_ptr = std::make_shared<TriangleMesh>(tmesh_filepath, LoadVector(tmesh, "Position"), LoadMaterial(tmesh), rotation, scale);
            
            return tmesh_ptr;
        }

        /**
         * @brief Creates a pointer to a rectangle object from yaml node.
         * 
         * @param rect Yaml node that contains properties of rectangle object
         * @return A pointer to a rectangle object 
         */
        std::shared_ptr<Rectangle> LoadRectangle(YAML::Node rect) {
            YAML::Node width_node = rect["Width"];
            YAML::Node height_node = rect["Height"];

            if (!width_node.IsDefined() || !height_node.IsDefined()) {
                throw ParameterNotFoundException(filepath_, rect.Mark().line);
            }
            
            float width = width_node.as<float>();
            float height = height_node.as<float>();

            if (width < 0)
            {
                throw NegativeDimensionException(filepath_, width, width_node.Mark().line);
            }

            if (height < 0)
            {
                throw NegativeDimensionException(filepath_, height, height_node.Mark().line);
            }
            
            std::shared_ptr<Rectangle> rect_ptr = std::make_shared<Rectangle>(LoadVector(rect, "Position"), width, height, LoadMaterial(rect));
            
            Vector rotation;
            try
            {
                rotation = LoadVector(rect, "Rotation");
            }
            catch(const InvalidKeyException& e)
            {
                std::cout << e.what() << std::endl;
                std::cout << "Standard rotation (0, 0, 0) will be used." << std::endl;
                rotation = Vector(0, 0, 0);
            }
            rotation = (rotation / 180.0) * M_PI;

            rect_ptr->rotate(rotation[0], Vector::UnitX());
            rect_ptr->rotate(rotation[1], Vector::UnitY());
            rect_ptr->rotate(rotation[2], Vector::UnitZ());
            
            return rect_ptr;
        }

        /**
         * @brief Loads all objects from yaml file 
         * 
         * @return A list of pointers to objects
         */
        std::list<std::shared_ptr<Object>> LoadObjects() { 
            YAML::Node objects = loadParams("Objects");
            std::list<std::shared_ptr<Object>> object_list;
            for (YAML::const_iterator it=objects.begin(); it!=objects.end(); ++it) {
                if((*it)["Object"]["Type"].as<std::string>() == "Ball") {
                    object_list.push_back(LoadBall((*it)["Object"]));
                }
                if((*it)["Object"]["Type"].as<std::string>() == "Box") {
                    object_list.push_back(LoadBox((*it)["Object"]));
                }
                if((*it)["Object"]["Type"].as<std::string>() == "TriangleMesh") {
                    object_list.push_back(LoadTriangleMesh((*it)["Object"]));
                }
                if((*it)["Object"]["Type"].as<std::string>() == "Rectangle") {
                    object_list.push_back(LoadRectangle((*it)["Object"]));
                }
            }
            return object_list;
        } 

        /**
         * @brief Creates a camera struct based on yaml file.
         * 
         * @return A camera struct 
         */
        Camera LoadCamera() {  
            YAML::Node params = loadParams("Camera");
            float fow = params["Fov"].as<float>();
            float focus = params["FocusDistance"].as<float>();
            if (fow < 0)
            {
                throw NegativeFOVException(filepath_, fow, params["Fov"].Mark().line);
            }
            else if (focus < 0)
            {
                throw NegativeFocusException(filepath_, focus, params["FocusDistance"].Mark().line);
            }
            Camera camera;
            camera.position = LoadVector(params, "Position");
            camera.direction = LoadVector(params, "Direction");
            camera.up = LoadVector(params, "Up");
            camera.fov = M_PI * fow;
            camera.focus_distance = focus;
            return camera;
        }

        /**
         * @brief Loads parameters from YAML file and throws exception if the key do not exist
         * 
         * @return parameters corresponding to key in the YAML file
         */
        YAML::Node loadParams(std::string key) {
            YAML::Node params = YAML::LoadFile(filepath_)[key];
            if (!params.IsDefined()) {
                throw InvalidKeyException(filepath_, key);
            }
            else
            {
                return params;
            }
        }



        std::string filepath_;
        std::shared_ptr<Scene> scene_;
};
