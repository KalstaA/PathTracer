/*#include <string>
#include <fstream>  
#include <iostream>
#include <string>
#include <../libs/Eigen/Dense>
#include <yaml-cpp/yaml.h>
#include "types.hpp"
#include "ball.hpp"*/
#pragma once


class FileLoader{
    public:
        FileLoader(std::string filepath) : filepath_(filepath) {} //Constructs a FileLoader object with filepath to the scene file

        Eigen::Vector3d LoadVector(YAML::Node coords) { //helper function to read and construct vectors
            Eigen::Vector3d vector;
            int i = 0;
            for (YAML::const_iterator it=coords.begin(); it!=coords.end(); ++it) {
                vector[i] = it->as<float>();
            }
            return vector;
        }

        Material LoadMaterial(YAML::Node material_node) { //helper function to read and construct materials
            Material material;
            material.color = LoadVector(material_node["Color"]);
            material.emission_color = LoadVector(material_node["EmissionColor"]);
            material.emission_strength = material_node["EmissionStrength"].as<float>();
            material.specularity = material_node["Specularity"].as<float>();
            return material;
        }

        Ball LoadBall(YAML::Node ball) { //helper function to read and construct balls
            return Ball(LoadVector(ball["Position"]), ball["Radius"].as<float>(), LoadMaterial(ball["Material"]));
        }

        std::list<Ball> LoadBalls() {
            YAML::Node objects = YAML::LoadFile(filepath_)["Objects"];
            std::list<Ball> ball_list;
            int i = 1;
            for (YAML::const_iterator it=objects.begin(); it!=objects.end(); ++it) {
                if(it->as<std::string>() == ("Ball" + std::to_string(i))) {
                    ball_list.push_back(LoadBall(objects["Ball" + std::to_string(i)]));
                }
                i++;
            }
            return ball_list;
        } 

        Camera LoadCamera() {  //Loads the camera from YAML-file
            YAML::Node params = YAML::LoadFile(filepath_);
            Camera camera;
            camera.position = LoadVector(params["Position"]);
            camera.position = LoadVector(params["Direction"]);
            camera.position = LoadVector(params["Up"]);
            camera.position = LoadVector(params["Right"]);
            camera.fov = M_PI * params["Fov"].as<float>();
            camera.focus_distance = params["FocusDistance"].as<float>();
            return camera;
        } 

        Scene loadSceneFile() {
            YAML::Node params = YAML::LoadFile(filepath_);
            Camera camera = LoadCamera();
            std::list<Ball> balls = LoadBalls();
            return Scene(camera, balls);
        }


    private:
        std::string filepath_;
};

