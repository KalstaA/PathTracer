#pragma once

#include "triangle.hpp"
#include "../libs/tiny_obj_loader/tiny_obj_loader.cc"

#include <iostream>
#include <vector>
#include <string>


class TriangleMesh : public Object {
public:
    TriangleMesh(std::string obj_filepath, Vector scenePos, Material m, int scale) : Object(scenePos, m) {
        unsigned long pos = obj_filepath.find_last_of("/");
        std::string basepath = obj_filepath.substr(0, pos+1);
        std::string obj_name = obj_filepath.substr(pos+1, obj_filepath.length());
        
        name = obj_name.substr(0, obj_name.length()-4);

        tinyobj::attrib_t attributes;
        std::string warnings;
        std::string errors;
        std::vector<Vector> vertices;

        bool r = tinyobj::LoadObj(&attributes, &objects, &materials, &warnings, &errors, obj_filepath.c_str(), basepath.c_str());

        if(!errors.empty()) {
            std::cout << "Error: " << errors << std::endl;
        }

        if(!r) {
            std::cout << "Failed to load object file." << std::endl;
            exit(1);
        }

        //Loop over objects (shapes)
        for(size_t o = 0; o < objects.size(); o++) {
            size_t o_offset = 0;
            //Loop over triangles (faces)
            for(size_t t = 0; t < objects[o].mesh.num_face_vertices.size(); t++) {
                int fv = objects[o].mesh.num_face_vertices[t];

                //Looping over vertices in this face
                for(size_t v = 0; v < fv; v++) {
                    tinyobj::index_t idx = objects[o].mesh.indices[o_offset + v];
                    tinyobj::real_t vx = attributes.vertices[3*idx.vertex_index+0];
                    tinyobj::real_t vy = attributes.vertices[3*idx.vertex_index+1];
                    tinyobj::real_t vz = attributes.vertices[3*idx.vertex_index+2];

                    Vector vertex = Vector(vx, vz, vy)*scale+scenePos;
                    vertices.push_back(vertex);
                }
                o_offset += fv;
            }
        }

        //Loops over vertices and creates triangles
        for(int i = 0; i < vertices.size()/3; ++i) {
            triangles.push_back(std::make_shared<Triangle>(vertices[i*3], vertices[i*3+1], vertices[i*3+2], m));
        }

        std::cout << "Object file: " << obj_name << ", succesfully opened!" << std::endl;

        objects.clear();
        materials.clear();
    }

    void collision(Ray& ray, Hit& rayHit, float& smallestDistance) {
        long t_size = triangles.size();
        for(long t = 0; t < t_size; t++){
            triangles[t]->collision(ray, rayHit, smallestDistance);
        }
        return;
    }

    void printInfo(std::ostream& out) const {
        out << "TriangleMesh object: " << name << ", at :" << this->getPosition().transpose() << ", with material: " << this-> getMaterial().name << std::endl;
    }

public:
    std::vector<tinyobj::shape_t> objects;
    std::vector<tinyobj::material_t> materials;
    std::vector<std::shared_ptr<Triangle>> triangles;
    std::string name;
};