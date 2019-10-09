# PA6: Assimp with Textures
### Christian Garcia

# Project description
This project added textures to a model from an obj file using Assimp. For the texture loading we used stb_image.h which is an included header so nothing special is required for building.

GROUP PROJECT: My teammate is Jake Lahr. Grade the project in Jake's repo.

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
A key: stop/change spin direction of the object

Left mouse click: stop/change orbit direction of the object

E key: increase size of the object

D key: decrease size of the object

