# Introduction
Simple Path Tracer project based on few external libraries:
- yaml-cpp (parsing .yaml files)
- Eigen (linear algebra)
- tiny_obj_loader (reading .obj files)
- SFML (graphics)
- OpenMP (CPU parallelization)

# Credits
This project is developed as a group project of Aalto University ELEC-A7151 Object-oriented Programming with C++ course in 2023. The responsible teacher for the course was Yusein Ali, and the project advisor was Markus Säynevirta.

Project was developed by:
- Aleksi Kalsta (@KalstaA)
- Eemeli Forsbom (@eemeliforsbom)
- Eetu Reijonen (@EetuReijonen)
- Henri-Mikael Suominen (@hemisu1)

# Building the project
In order to build the project you need to have CMake version 3.19 or higher and OpenMP, such that CMake is able to find it. You can build the project by simply running the following commands from the project root, where CMakeLists.txt is located and it will fetch other dependencies from GitHub.
```
cmake -S . -B build/ -DCMAKE_BUILD_TYPE=Release
cd build && make
```

# Running renders for example scenes
There is a scenes/ directory with few example scenes to render. You can use the program to render these by running the command
```
./PathTracer ..scenes/<scene_name> <res X> <res Y> <rays per pixel> <max bounces> <image name>
```
from the build directory. For example
```
./PathTracer ..scenes/mirrorRoom.yaml 1920 1080 50 15 image.png
```
should work.
