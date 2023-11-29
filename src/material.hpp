#ifndef MATERIAL_CLASS
#define MATERIAL_CLASS

#include "types.hpp"
#include "randomgenerator.hpp"

#include <vector>
#include <iostream>
#include <yaml-cpp/yaml.h>


#define WHITE Color(1.0, 1.0, 1.0)


class Material {
    private:
        Color color_;
        bool emitting_;
        Color emission_color_;
        float emission_strength_;
        std::string name_;

    public:
        RandomGenerator rnd_;

        // Getters
        Color getColor() { return color_; }
        Color getEmColor() { return emission_color_; }
        float getEmStrength() { return emission_strength_; }
        bool isEmitting() { return emitting_; }
        std::string getName() {return name_; }

        /**
        * @brief Constructor for material class
        * 
        * @param color Material color as RGB value
        * @param emission_strength Emission strength of the material
        * @param emission_color Emission color of the material
        */
        Material(Color color = WHITE, std::string name = "UNDEFINED", float emission_strength = 0.0, Color emission_color = WHITE) : 
            color_(color),
            emission_strength_(emission_strength),
            emission_color_(emission_color),
            name_(name) {
            
            // Boolean value, to determine if material is emitting
            (emission_strength > 0) ? (emitting_ = true) : (emitting_ = false);
        }

        /**
        * @brief Pure virtual function that updates the ray according to the properties of the material
        * 
        * @param ray ray that did hit the material
        */
        virtual void updateRay(Ray& ray, Hit& hit) = 0;

};

class Diffuse : public Material {
    private:

        /**
        * @brief Updates the direction of the reflected ray from the diffuse material
        * 
        * randomDirection() and ray.normal are both normalized so this gives a random vector whose probability distribution
        * is weighted towards the ray.normal.
        * 
        * @param ray ray that did hit the material
        */
        void diffuseDir(Ray& ray, Hit& hit) {
            ray.direction = (rnd_.randomDirection() + hit.normal).normalized();
            return;
        }

        /**
        * @brief Updates the color of the ray after interacting with the diffuse material
        * 
        * Component wise product with the original color and the color of this diffuse material
        * 
        * @param ray ray that did hit the material
        */
        void diffuseColor(Ray& ray) {
            ray.color = ray.color.cwiseProduct(getColor());
            return;
        }

        /**
        * @brief Updates the light of the ray that did hit diffuse material
        * 
        * If material is not emitting, then computes nothing, else takes component wise
        * product between emitted light and ray color and updates light of the ray.
        * 
        * @param ray ray that did hit the material
        */
        void diffuseEmission(Ray& ray) {
            if (!isEmitting()) { return; } // If not emitting do nothing
            else {
                ray.light += (getEmStrength() * getEmColor()).cwiseProduct(ray.color);
                return;
            }
        }

    public:

        /**
        * @brief Constructor for Diffuse class
        * 
        * @param color Material color as RGB value
        * @param emission_strength Emission strength of the material
        * @param emission_color Emission color of the material
        */
        Diffuse(Color color = WHITE, std::string name = "UNDEFINED", float emission_strength = 0.0, Color emission_color = WHITE) :
        Material(color, name, emission_strength, emission_color) {}

        /**
        * @brief Updates the ray according to properties of diffuse material
        * 
        * @param ray ray that did hit the material
        */
        void updateRay(Ray& ray, Hit& hit) {
            ray.origin = hit.point;
            diffuseDir(ray, hit);
            diffuseColor(ray);
            diffuseEmission(ray);
            return;
        }
};

class Mirror : public Material {
    private:

        float specularity_;

        /**
        * @brief Computes the direction of the reflected ray from the mirror material
        * 
        * Direction of the completely reflected ray
        * 
        * @param ray ray that did hit the material
        * @return vector towards the direction of the reflected ray
        */
        Vector reflectionDir(Ray& ray, Hit& hit) {
            return ray.direction - 2 * ray.direction.dot(hit.normal) * hit.normal;
        }

        /**
        * @brief Computes the direction of the reflected ray from the diffuse material
        * 
        * randomDirection() and ray.normal are both normalized so this gives a random vector whose probability distribution
        * is weighted towards the ray.normal.
        * 
        * @param ray ray that did hit the material
        * @return vector towards the direction of the diffused ray
        */
        Vector diffuseDir(Ray& ray, Hit& hit) {
            return (rnd_.randomDirection() + hit.normal).normalized();
        }

        /**
        * @brief Updates the color of the ray after interacting with the mirror material
        * 
        * @param ray ray that did hit the material
        */
        void mirrorColor(Ray& ray) {
            ray.color = ray.color.cwiseProduct(getColor());
            return;
        }


    public:

        /**
        * @brief Constructor for Mirror class
        * 
        * @param color Material color as RGB value
        * @param emission_strength Emission strength of the material
        * @param emission_color Emission color of the material
        */
        Mirror(Color color = WHITE, std::string name = "UNDEFINED", float specularity = 1.0) :
            Material(color, name, 0.0, WHITE), specularity_(specularity) {}

        float getSpecularity() { return specularity_; }

        /**
        * @brief Updates the ray according to properties of mirror material
        * 
        * @param ray ray that did hit the material
        */
        void updateRay(Ray& ray, Hit& hit) {
            ray.origin = hit.point;
            Vector reflectedRay = reflectionDir(ray, hit);
            Vector diffusedRay = diffuseDir(ray, hit);
            // Weight direction of the reflection based on specularity
            ray.direction = diffusedRay + getSpecularity() * (reflectedRay - diffusedRay);
            mirrorColor(ray);
            return;
        }
};

#endif