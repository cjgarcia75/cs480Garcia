# PA3: Assimp and Model Loading
### Christian Garcia

# Project description
This project added a model from an obj file using Assimp.

GROUP PROJECT: My teammate is Jake Lahr. Both of the repos have a working project.

## Extra Credit
Color has been added to the model in colors() in object.cpp

## Building
To build and run this program, navigate to the correct directory. Make a new 'build' directory, cmake in previous directory, then make in the build directory. Then run the program with the shader file names and object file name as command line arguments.

Example:
```
mkdir build
cd build
cmake ..
make
./Tutorial cube_shader-v.txt cube_shader-f.txt tray.obj
```

Note: Make sure that the vertex shader (cube_shader-v.txt) is first in the command line.

### Controls
A key: stop/change spin direction of planet

S key: stop/change spin direction of moon

Left mouse click: stop/change orbit direction of planet

Right mouse click: stop/change orbit direction of moon

E key: increase size of planet

D key: decrease size of planet

R key: increase size of moon

F key: decrease size of moon

T key: increase speed of planet spin

G key: decrease speed of planet spin

Y key: increase speed of planet orbit

H key: decrease speed of planet orbit

U key: increase speed of moon spin

J key: decrease speed of moon spin

I key: increase speed of moon orbit

K key: decrease speed of moon orbit

