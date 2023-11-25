#ifndef FILELOADER_TEST
#define FILELOADER_TEST

#include <gtest/gtest.h>
#include <string>
#include <memory>

#include "fileloader.hpp"
#include "fileloader_ex.hpp"
#include "types.hpp"
#include "scene.hpp"

// Path to test files folder
std::string PATH = "../tests/yaml_testfiles/";
// Pi
#define M_PI 3.14159265358979323846264338327950288

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
    Vector up1 = (**obj).getUp();
    Vector forw1 = (**obj).getForward();
    Material material1 = (**obj).getMaterial();
    //float rad1 = (**obj).getRadius();

    ++obj;
    Vector position2 = (**obj).getPosition();
    Vector up2 = (**obj).getUp();
    Vector forw2 = (**obj).getForward();
    Material material2 = (**obj).getMaterial();
    //float rad2 = (**obj).getRadius();

    EXPECT_TRUE(position1 == Vector(5, 0, 0));
    EXPECT_TRUE(up1 == Vector(0, 0, 1));
    EXPECT_TRUE(forw1 == Vector(1, 0, 0));
    EXPECT_TRUE(position2 == Vector(6, 0, -31));
    EXPECT_TRUE(up2 == Vector(0, 0, 1));
    EXPECT_TRUE(forw2 == Vector(1, 0, 0));

    //EXPECT_EQ(1.299, rad1);
    //EXPECT_GE(1.301, rad1);
    //EXPECT_EQ(29.999, rad2);
    //EXPECT_GE(30.001, rad2);

    EXPECT_TRUE((material1.color - Color(1, 0, 0)).norm() < 0.001);
    EXPECT_EQ((material1.emission_color - Color(0.3, 0.3, 0.3)).norm(), 0.001);
    EXPECT_LE(0.499, material1.emission_strength);
    EXPECT_GE(0.501, material1.emission_strength);
    EXPECT_LE(0.989, material1.specularity);
    EXPECT_GE(0.991, material1.specularity);
    EXPECT_STREQ(material1.name.c_str(), "RED DIFFUSE");

    EXPECT_TRUE((material2.color - Color(0.5, 0.5, 0.5)).norm() < 0.001);
    EXPECT_TRUE((material2.emission_color - Color(1, 1, 1)).norm() < 0.001);
    EXPECT_LE(-0.001, material2.emission_strength);
    EXPECT_GE(0.001, material2.emission_strength);
    EXPECT_LE(0.320, material2.specularity);
    EXPECT_GE(0.322, material2.specularity);
    EXPECT_STREQ(material2.name.c_str(), "GREY DIFFUSE");
}

#endif
