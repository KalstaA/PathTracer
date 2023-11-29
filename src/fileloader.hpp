#pragma once

#include <yaml-cpp/yaml.h>
#include <string>
#include <exception>
#include <memory>
#include <sys/stat.h>

#include "box.hpp"
#include "ball.hpp"
#include "scene.hpp"
#include "types.hpp"
#include "fileloader_ex.hpp"

/**
 * @brief Implements a class for reading yaml scene files.
 * 
 */

class FileLoader{
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

        /*
        Problem in the memory management was that in the loadSceneFile we create a Scene object locally and return it by copy, meaning we
        can not directly delete objects in Scene destructor, since it will destroy objects immediately after leaving loadSceneFile
        function scope (and delete the objects where copied pointers are pointing). Also in renderer we used a default operator= which will
        perform member wise assignment for all the object members (including all the pointers of deleted Objects). Similarly "MyClass a = b" performs
        a member wise copy constructor for b, which by default takes the pointers pointing to the objects that will be deleted after
        leaving the function scope.

        Solution:
        Created scene object with "std::make_shared<Scene>" command and save/use the pointer to scene object instead of the object itself (also changed the code
        from other places accordingly). Then *scene_ is not destroyed after exiting a loadSceneFile function scope and used shared_ptr so that we don't need to
        delete scene object manually (It might not be generated in the case of exception, which creates a segfault and it can be shared with renderer)

        Other possible solution:
        Define operator overloads to copy and assignment for Scene and Object classes, such that the copying works correctly.
        */

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
         * @brief Creates a material struct based on properties given in yaml node.
         * 
         * @param material_node An yaml node that points to a key that has possible key-value pairs containing
         * material values 
         * @return A material struct  
         */
        Material LoadMaterial(YAML::Node node) {
            Material material;
            YAML::Node material_node = node["Material"];
            if (!material_node.IsDefined()) {
                throw MaterialNotFoundException(filepath_, node.Mark().line);
            }
            if(material_node["Color"]) {
                material.color = LoadVector(material_node, "Color");
            }
            if(material_node["EmissionColor"]) {
             material.emission_color = LoadVector(material_node, "EmissionColor");
            }
            if(material_node["EmissionStrength"]) {
                material.emission_strength = material_node["EmissionStrength"].as<float>();
            }
            if(material_node["Specularity"]) {
                material.specularity = material_node["Specularity"].as<float>();
            }
            if(material_node["Name"]) {
                material.name = material_node["Name"].as<std::string>();
            }
            return material;
        }

        /**
         * @brief Creates a pointer to a ball object from yaml node.
         * 
         * @param ball Yaml node that contains properties of ball object
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
            box_ptr->rotate(rotation[0], Vector::UnitX());
            box_ptr->rotate(rotation[1], Vector::UnitY());
            box_ptr->rotate(rotation[2], Vector::UnitZ());
            
            return box_ptr;
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
