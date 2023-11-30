#ifndef MATERIAL_CLASS
#define MATERIAL_CLASS

#include "types.hpp"
#include "randomgenerator.hpp"

#include <vector>
#include <iostream>
#include <yaml-cpp/yaml.h>

// Default color
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
        Material(Color color, std::string name, float emission_strength, Color emission_color) : 
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
        void updateColor(Ray& ray) {
            ray.color = ray.color.cwiseProduct(getColor());
            return;
        }

        /**
        * @brief Updates the light of the ray that did hit the diffuse material.
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
        Diffuse(Color color, std::string name, float emission_strength, Color emission_color) :
        Material(color, name, emission_strength, emission_color) {}

        /**
        * @brief Updates the ray according to properties of diffuse material
        * 
        * @param ray ray that did hit the material
        */
        void updateRay(Ray& ray, Hit& hit) {
            ray.origin = hit.point;
            diffuseDir(ray, hit);
            updateColor(ray);
            diffuseEmission(ray);
            return;
        }
};

class Reflective : public Material {
    private:

        float specularity_;

        /**
        * @brief Updates the color of the ray after interacting with the mirror material
        * 
        * @param ray ray that did hit the material
        */
        void updateColor(Ray& ray) {
            ray.color = ray.color.cwiseProduct(getColor());
            return;
        }


    public:

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
        * @brief Constructor for Reflective class material
        * 
        * @param color Material color as RGB value
        * @param emission_strength Emission strength of the material
        * @param emission_color Emission color of the material
        */
        Reflective(Color color, std::string name, float specularity) :
            Material(color, name, 0.0, WHITE), specularity_(specularity) {}

        float getSpecularity() { return specularity_; }

        /**
        * @brief Updates the ray according to properties of mirror material
        * 
        * @param ray ray that did hit the material
        */
        void updateRay(Ray& ray, Hit& hit) {
            ray.origin = hit.point;
            updateColor(ray);
            Vector reflectedRay = reflectionDir(ray, hit);
            Vector diffusedRay = diffuseDir(ray, hit);
            // Weight direction of the reflection based on specularity
            ray.direction = diffusedRay + getSpecularity() * (reflectedRay - diffusedRay);
            return;
        }
};

class ClearCoat : public Reflective {
    private:

        float clearcoat_;
        Color clearcoat_color_;

        /**
        * @brief Computes if the ray performs the clear cout bounce
        * 
        * @return bool true if clear cout bounce is performed and false otherwise
        */
        bool clearCoatBounce() { return (clearcoat_ >= rnd_.randomZeroToOne()); }

        /**
        * @brief Updates the color of the ray after interacting with the mirror material
        * 
        * @param ray ray that did hit the material
        */
        void updateColor(Ray& ray, bool bounce) {
            ray.color = ray.color.cwiseProduct(bounce ? getClearCoatColor() : getColor());
            return;
        }


    public:

        Color getClearCoatColor() { return clearcoat_color_; }

        /**
        * @brief Constructor for ClearCoat class
        * 
        * @param color Material color as RGB value
        * @param emission_strength Emission strength of the material
        * @param emission_color Emission color of the material
        */
        ClearCoat(Color color, std::string name, float specularity, float clearcoat, Color clearcoat_color) :
            Reflective(color, name, specularity), clearcoat_(clearcoat), clearcoat_color_(clearcoat_color) {}

        /**
        * @brief Updates the ray according to properties of mirror material
        * 
        * @param ray ray that did hit the material
        */
        void updateRay(Ray& ray, Hit& hit) {
            bool bounce = clearCoatBounce();
            ray.origin = hit.point;
            updateColor(ray, bounce);
            if (bounce) {
                ray.direction = diffuseDir(ray, hit) + getSpecularity() * (reflectionDir(ray, hit) - diffuseDir(ray, hit));
            }
            else {
                ray.direction = diffuseDir(ray, hit);
            }
            return;
        }
};

class Refractive : public Material {
    private:
        float refraction_ratio_; // From outside to inside the material

        // Theta is angle between incoming ray and surface normal (vectors has to be normalized)
        float cosTheta(Vector a, Vector b) {
            return a.dot(b);
        }

        float sinTheta(Vector a, Vector b) {
            float cos = cosTheta(a, b);
            return sqrt(1 - cos*cos);
        }

        /**
        * @brief Computes the perpendicular part of the refracted ray with respect to inside normal
        * 
        * @param in normalized vector towards the Direction of the incoming ray
        * @param normal Surface normal (outside direction)
        * @param ref_ratio refraction ratio, which is either refraction_ratio_ or 1/refraction_ratio_
        * @return Vector towards the direction of the perpendicular part of the refracted ray
        */
        Vector refractionPerpendicular(Vector in, Vector normal, float ref_ratio) {
            float cos_theta = cosTheta(-in, normal);
            return ref_ratio * (in + cos_theta * normal);
        }

        /**
        * @brief Computes the direction of the refracted ray
        * 
        * @param ray Incoming ray
        * @param hit Information about hit
        * @param ref_ratio refraction ratio, which is either refraction_ratio_ or 1/refraction_ratio_
        * @return Vector towards the direction of the refracted ray
        */
        Vector refractionDir(Ray& ray, Hit& hit, float ref_ratio) {
            Vector perpendicular = refractionPerpendicular(ray.direction, hit.normal, ref_ratio);
            Vector parallel = -sqrt(1 - perpendicular.dot(perpendicular)) * hit.normal;
            return perpendicular + parallel;
        }

        /**
        * @brief Polynomial approximation for reflectance probability as the function of cosine theta and reflection ratio
        * 
        * @param cos_theta Cosine of the angle between the incoming ray and surface normal
        * @param ref_ratio Refraction ratio for in to out
        * @return floating point number representing the probability that the ray reflects
        */
        float reflectance(float cos_theta, float ref_ratio) {
            float r0 = (1 - ref_ratio) / (1 + ref_ratio);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cos_theta), 5);
        }

        /**
        * @brief Computes the direction of the reflected ray
        * 
        * Direction of the completely reflected ray
        * 
        * @param ray ray that did hit the material
        * @return vector towards the direction of the reflected ray
        */
        Vector reflectionDir(Ray& ray, Hit& hit) {
            return ray.direction - 2 * ray.direction.dot(hit.normal) * hit.normal;
        }

        void updateColor(Ray& ray) {
            ray.color = ray.color.cwiseProduct(getColor());
            return;
        }
        
    public:

        /**
        * @brief Constructor for Refractive class
        * 
        * @param color Material color as RGB value
        * @param name Name of the material
        * @param refraction_ratio Refraction ratio from outside to inside the material
        */
        Refractive(Color color, std::string name, float refraction_ratio) :
            Material(color, name, 0.0, WHITE), refraction_ratio_(refraction_ratio) {}

        /**
        * @brief Updates the ray according to properties of Refractive material
        * 
        * @param ray Ray that did hit the material
        * @param hit Information about the hit point
        */
        void updateRay(Ray& ray, Hit& hit) {
            ray.origin = hit.point;
            updateColor(ray);

            // Real refraction ratio depends on the direction
            float ref_ratio = hit.inside_material ? 1 / refraction_ratio_ : refraction_ratio_;

            // Real glass reflects depending on the intersection angle and refraction ratio
            float cos_theta = cosTheta(-ray.direction, hit.normal);
            bool must_reflect = ref_ratio * sinTheta(-ray.direction, hit.normal) > 1;
            float reflectanceProb = reflectance(cos_theta, ref_ratio);

            if (must_reflect || reflectanceProb > rnd_.randomZeroToOne()) {
                // Must reflect
                Vector reflectedDir = reflectionDir(ray, hit);
                ray.direction = reflectedDir;
            } else {
                // Can refract
                Vector refractedDir = refractionDir(ray, hit, ref_ratio);
                ray.direction = refractedDir;
            }
            return;
        }
        
};

#endif