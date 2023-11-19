import os

# Build yaml library
os.chdir("libs/yaml-cpp-master/")
os.system("mkdir buildlib")
os.system("cmake -S . -B buildlib/")
os.chdir("buildlib/")
os.system("make")

# Build project
os.chdir("../../../")
os.system("mkdir build")
os.system("cmake -S . -B build/")
os.chdir("build")
os.system("make")
