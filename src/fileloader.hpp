#pragma once

#include <yaml-cpp/yaml.h>

#include "ball.hpp"
#include "scene.hpp"
#include "types.hpp"

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

        FileLoader(std::string filepath) : filepath_(filepath) {} 

        /*
        Problem in the memory management was that in the loadSceneFile we create a Scene object locally and return it by copy, meaning we
        can not directly delete objects in Scene destructor, since it will destroy objects immediately after leaving loadSceneFile
        function scope (and delete the objects where copied pointers are pointing). Also in renderer we used a default operator= which will
        perform member wise assignment for all the object members (including all ready deleted Objects). Similarly "MyClass a = b" performs
        a member wise copy constructor for b, which by default takes the pointers pointing to the objects that will be deleted after
        leaving the function scope.

        Solution:
        Created scene object with "new" command and save/use the pointer to scene object instead of the object itself (also changed the code
        from other places accordingly). Then scene_ needs to be deleted when FileLoader is deleted.

        Other possible solution:
        Define operator overloads to copy and assignment for Scene and Object classes, such that the copying works correctly.
        */

        /**
         * @brief Loads a scene from the file specified in filepath and returns a pointer to this scene.
         * 
         * @return A pointer to scene object
         */
        Scene* loadSceneFile() {
            YAML::Node params = YAML::LoadFile(filepath_);
            Camera camera = LoadCamera();
            std::list<Object*> objects = LoadObjects();
            scene_ = new Scene(camera, objects);
            LoadEnvironment(scene_);
            return scene_;
        }

        /**
         * @brief Destructor for FileLoader object
         */
        ~FileLoader() {
            delete scene_;
        }

        /**
         * @brief Takes a pointer to a scene object as an input and sets the environment of the scene to values specified in
         * scene file. If scene file has no "Environment" node, the environment is set to default values.
         * 
         * @param scene A  reference to a scene object
         */
        void LoadEnvironment(Scene* scene) {
            if(YAML::LoadFile(filepath_)["Environment"]) {
                YAML::Node environment_node = YAML::LoadFile(filepath_)["Environment"];
                Eigen::Vector3d skyColor = LoadVector(environment_node["SkyColor"]);
                Eigen::Vector3d horizonColor = LoadVector(environment_node["HorizonColor"]);
                Eigen::Vector3d groundColor = LoadVector(environment_node["GroundColor"]); 
                (*scene).getEnvironment().setSky(skyColor, horizonColor, groundColor);
            }else {
                (*scene).getEnvironment().setSky();
            }
            
        }

    private:
        /**
         * @brief A helper function to create vectors from yaml sequences.
         * 
         * @param coords An yaml node that points to a key that has a sequence as a value 
         * @return A vector based on values in node
         */
        Eigen::Vector3d LoadVector(YAML::Node coords) { 
            Eigen::Vector3d vector;
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
        Material LoadMaterial(YAML::Node material_node) { 
            Material material;
            if(material_node["Color"]) {
                material.color = LoadVector(material_node["Color"]);
            }
            if(material_node["EmissionColor"]) {
             material.emission_color = LoadVector(material_node["EmissionColor"]);
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
        Object* LoadBall(YAML::Node ball) { 
            Ball* ball_ptr = new Ball(LoadVector(ball["Position"]), ball["Radius"].as<float>(), LoadMaterial(ball["Material"]));
            return ball_ptr;
        }

        /**
         * @brief Loads all objects from yaml file 
         * 
         * @return A list of pointers to objects
         */
        std::list<Object*> LoadObjects() { 
            YAML::Node objects = YAML::LoadFile(filepath_)["Objects"];
            std::list<Object*> object_list;
            for (YAML::const_iterator it=objects.begin(); it!=objects.end(); ++it) {
                if((*it)["Object"]["Type"].as<std::string>() == "Ball") {
                    object_list.push_back(LoadBall((*it)["Object"]));
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
            YAML::Node params = YAML::LoadFile(filepath_)["Camera"];
            Camera camera;
            camera.position = LoadVector(params["Position"]);
            camera.direction = LoadVector(params["Direction"]);
            camera.up = LoadVector(params["Up"]);
            camera.fov = M_PI * params["Fov"].as<float>();
            camera.focus_distance = params["FocusDistance"].as<float>();
            return camera;
        } 


        std::string filepath_;
        Scene* scene_;
};
