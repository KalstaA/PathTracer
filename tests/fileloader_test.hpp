#ifndef FILELOADER_TEST
#define FILELOADER_TEST

#include <gtest/gtest.h>
#include <string>
#include <memory>
#include <Eigen/Dense>

#include "fileloader.hpp"
#include "fileloader_ex.hpp"
#include "types.hpp"
#include "scene.hpp"
#include "material.hpp"

// Path to test files folder
std::string PATH = "../tests/yaml_testfiles/";
// Pi
#define M_PI 3.14159265358979323846264338327950288

// Define new macro that checks the given value is in range
#define EXPECT_IN_RANGE(VAL, MIN, MAX) \
    EXPECT_GE((VAL), (MIN));           \
    EXPECT_LE((VAL), (MAX))

// Helper function to compute distance between two vectors
float distance(Eigen::Vector3d v1, Eigen::Vector3d v2) { return (v1 - v2).norm(); }

// Test FileLoader constructor
TEST(FILELOADER, Constructor) {
    std::string file = PATH + "yaml_exists.yaml";
    FileLoader fileloader1(file);
    EXPECT_STREQ(fileloader1.getFilepath().c_str(), file.c_str());
    // Test that the thrown exception is correct and the error message is correct
    std::string fpath = "file/do/not/exist.yaml";
    std::string msg = "FileLoader exception caught:\nInvalid filepath: " + fpath;
    EXPECT_THROW({
        try
        {
            FileLoader fileloader2(fpath);
        }
        catch(const InvalidFilepathException& e)
        {
            EXPECT_STREQ(msg.c_str(), e.what());
            throw;
        }
        
    }, InvalidFilepathException);
}


// Loading objects do not implement all parameters
// What would be best way to access radius of the balls?
TEST(FILELOADER, CorrectLoadScene) {
    FileLoader fileloader(PATH + "correctloadscene.yaml");
    std::shared_ptr<Scene> scene = fileloader.loadSceneFile();
    Camera cam = (*scene).getCamera();
    Environment env = (*scene).getEnvironment();
    std::list<std::shared_ptr<Object>> objects = (*scene).getObjects();

    // Test camera
    EXPECT_TRUE(cam.position == Point(0, 0, 0));
    EXPECT_TRUE(cam.direction == Vector(1, 0, 0));
    EXPECT_TRUE(cam.up == Vector(0, 0, 1));
    EXPECT_LE(0.33*M_PI - 0.001, cam.fov);
    EXPECT_GE(0.33*M_PI + 0.001, cam.fov);
    EXPECT_EQ(5, cam.focus_distance);

    // Test environment
    EXPECT_TRUE((env.getHorizonColor() - Color(0.7, 0.8, 0.8)).norm() < 0.001);
    EXPECT_TRUE((env.getSkyColor() - Color(0.2, 0.5, 1.0)).norm() < 0.001);
    EXPECT_TRUE((env.getGroundColor() - Color(0.1, 0.1, 0.1)).norm() < 0.001);

    // Test objects
    auto obj = objects.begin();
    Vector position1 = (**obj).getPosition();
    std::shared_ptr<Material> material1 = (**obj).getMaterial();

    std::advance(obj, 1);
    Vector position2 = (**obj).getPosition();
    std::shared_ptr<Material> material2 = (**obj).getMaterial();

    EXPECT_LE(distance(position1, Vector(5, 0, 0)), 0.001);
    EXPECT_LE(distance(position2, Vector(6, 0, -31)), 0.001);

    EXPECT_LE(distance(material1->getColor(), Color(1, 0, 0)), 0.001);
    //EXPECT_LE(distance(material1->getEmColor(), Color(0.3, 0.3, 0.3)), 0.001);
    //EXPECT_IN_RANGE(material1.emission_strength, 0.499, 0.501);
    //EXPECT_IN_RANGE(material1.specularity, 0.989, 0.991);
    EXPECT_STREQ(material1->getName().c_str(), "RED DIFFUSE");

    EXPECT_LE(distance(material2->getColor(), Color(0.5, 0.5, 0.5)), 0.001);
    //EXPECT_LE(distance(material2->getEmColor(), Color(1, 1, 1)), 0.001);
    //EXPECT_IN_RANGE(material2.emission_strength, 0.099, 0.101);
    //EXPECT_IN_RANGE(material2.specularity, 0.320, 0.322);
    EXPECT_STREQ(material2->getName().c_str(), "GREY DIFFUSE");
}

TEST(FILELOADER, InvalidKey) {
    // Initialize filepaths and file loaders
    std::string fpath1 = PATH + "expect_invalid_key1.yaml";
    std::string fpath2 = PATH + "expect_invalid_key2.yaml";
    FileLoader fileloader1(fpath1);
    FileLoader fileloader2(fpath2);

    // Desired error messages
    std::string msg1 = "FileLoader exception caught:\nInvalid key: Objects, for file: " + fpath1;
    std::string msg2 = "FileLoader exception caught:\nInvalid key: Direction, for file: " + fpath2;

    // Should throw invalid key exception
    EXPECT_THROW({
        try
        {
            std::shared_ptr<Scene> scene = fileloader1.loadSceneFile();
        }
        catch(const InvalidKeyException& e)
        {
            EXPECT_STREQ(msg1.c_str(), e.what());
            throw;
        }
        
    }, InvalidKeyException);

    // Should throw invalid key exception
    EXPECT_THROW({
        try
        {
            std::shared_ptr<Scene> scene = fileloader2.loadSceneFile();
        }
        catch(const InvalidKeyException& e)
        {
            EXPECT_STREQ(msg2.c_str(), e.what());
            throw;
        }
        
    }, InvalidKeyException);
}

TEST(FILELOADER, NegativeRadius) {
    // Initialize filepaths and file loaders
    std::string fpath1 = PATH + "negative_radius.yaml";
    FileLoader fileloader1(fpath1);

    // Desired error message
    std::string msg1 = "FileLoader exception caught:\nNegative radius (" +
                        std::to_string(-1.3) + ") in file: " +
                        fpath1 + ", on line: 37";

    // Should throw negative radius exception
    EXPECT_THROW({
        try
        {
            std::shared_ptr<Scene> scene = fileloader1.loadSceneFile();
        }
        catch(const NegativeRadiusException& e)
        {
            EXPECT_STREQ(msg1.c_str(), e.what());
            throw;
        }
        
    }, NegativeRadiusException);
}

TEST(FILELOADER, InvalidSizeVector) {
    // Initialize filepaths and file loaders
    std::string fpath1 = PATH + "invalid_size_vector.yaml";
    FileLoader fileloader1(fpath1);

    // Desired error message
    std::string msg1 = "FileLoader exception caught:\nInvalid vector of size " +
                        std::to_string(2) + " in file: " + fpath1 + ", on line: 34";

    // Should throw negative radius exception
    EXPECT_THROW({
        try
        {
            std::shared_ptr<Scene> scene = fileloader1.loadSceneFile();
        }
        catch(const InvalidSizeVectorException& e)
        {
            EXPECT_STREQ(msg1.c_str(), e.what());
            throw;
        }
        
    }, InvalidSizeVectorException);
}

TEST(FILELOADER, InvalidCamera) {
    // Initialize filepaths and file loaders
    std::string fpath1 = PATH + "invalid_fow.yaml";
    std::string fpath2 = PATH + "invalid_focus.yaml";
    FileLoader fileloader1(fpath1);
    FileLoader fileloader2(fpath2);

    // Desired error messages
    std::string msg1 = "FileLoader exception caught:\nNegative FOW: " + std::to_string(-0.33) +
                        ", in file: " + fpath1 + ", on line: " + std::to_string(13);
    std::string msg2 = "FileLoader exception caught:\nNegative focus distance: " +
                        std::to_string(-5.0) + " in file: " + fpath2 +
                        ", on line: " + std::to_string(14);

    // Should throw invalid key exception
    EXPECT_THROW({
        try
        {
            std::shared_ptr<Scene> scene = fileloader1.loadSceneFile();
        }
        catch(const NegativeFOVException& e)
        {
            EXPECT_STREQ(msg1.c_str(), e.what());
            throw;
        }
        
    }, NegativeFOVException);

    // Should throw invalid key exception
    EXPECT_THROW({
        try
        {
            std::shared_ptr<Scene> scene = fileloader2.loadSceneFile();
        }
        catch(const NegativeFocusException& e)
        {
            EXPECT_STREQ(msg2.c_str(), e.what());
            throw;
        }
        
    }, NegativeFocusException);
}

TEST(FILELOADER, NoMaterial) {
    // Initialize filepaths and file loaders
    std::string fpath1 = PATH + "no_material.yaml";
    FileLoader fileloader1(fpath1);

    // Desired error message
    std::string msg1 = "FileLoader exception caught:\nMaterial not defined for object at line: " +
                        std::to_string(32) + ", in file: " + fpath1 + ".";

    // Should throw negative radius exception
    EXPECT_THROW({
        try
        {
            std::shared_ptr<Scene> scene = fileloader1.loadSceneFile();
        }
        catch(const MaterialNotFoundException& e)
        {
            EXPECT_STREQ(msg1.c_str(), e.what());
            throw;
        }
        
    }, MaterialNotFoundException);
}

TEST(FILELOADER, NoRadius) {
    // Initialize filepaths and file loaders
    std::string fpath1 = PATH + "no_radius.yaml";
    FileLoader fileloader1(fpath1);

    // Desired error message
    std::string msg1 = "FileLoader exception caught:\nRadius not defined for ball at line: " +
                        std::to_string(53) + ", in file: " + fpath1 + ".";

    // Should throw negative radius exception
    EXPECT_THROW({
        try
        {
            std::shared_ptr<Scene> scene = fileloader1.loadSceneFile();
        }
        catch(const RadiusNotFoundException& e)
        {
            EXPECT_STREQ(msg1.c_str(), e.what());
            throw;
        }
        
    }, RadiusNotFoundException);
}

TEST(FILELOADER, InvalidMaterialType) {
    // Initialize filepaths and file loaders
    std::string fpath1 = PATH + "invalid_material_type.yaml";
    FileLoader fileloader1(fpath1);

    // Desired error message
    std::string msg1 = "FileLoader exception caught:\nInvalid material type in file: " +
                        fpath1 + ", for material starting on line: 64.";

    // Should throw negative radius exception
    EXPECT_THROW({
        try
        {
            std::shared_ptr<Scene> scene = fileloader1.loadSceneFile();
        }
        catch(const InvalidMaterialTypeException& e)
        {
            EXPECT_STREQ(msg1.c_str(), e.what());
            throw;
        }
        
    }, InvalidMaterialTypeException);
}

#endif
