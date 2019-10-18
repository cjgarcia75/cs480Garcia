# PA7: Solar System
### Christian Garcia

# Project description
This project is an interactive solar system using Assimp to load models and stb_image.h to load textures

GROUP PROJECT: My teammate is Jake Lahr. Grade this project.

## Extra Credit
We used a config file for the values of the planets.

Menu.

Live adjustment of simulation speed.

## Building
To build and run this program, navigate to the correct directory. Make a new 'build' directory, cmake in previous directory, then make in the build directory. Then run the program with the shader file names and object file name as command line arguments.

Example:
```
mkdir build
cd build
cmake ..
make
./Solar
```

## Controls
R: reset camera.

0-9: focus camera on the sun and planets.

Q, W, E: Change speed.

