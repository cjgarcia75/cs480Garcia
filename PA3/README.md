# PA3: Moons

# Project description
This project added a moon to the previous project as well as the ability to control the size of the two cubes and the speed of the spin and rotation of both cubes. The controls are handled using a switch statement, the speeds are stored in int arrays, and the sizees are stored in a float array. The moon uses the model matrix of the first cube, after its translate and before its rotate, to stay orbiting the first cube.

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev
```

### Mac OSX
Installation of brew is suggested to easily install the libs. Ensure that the latest version of the Developer Tools is installed.
```bash
brew install glew glm sdl2
```

## Building and Running
To build this project there are two options. One is to use CMake which makes including new libraries easier, and handles new files added automatically to the src and include directory. CMake is a small new learning curve but makes things easier in the future.
The second option is to use the provided Makefile which is used as usual.

Running the make in a separate directory will allow easy cleanup of the build data, and an easy way to prevent unnecessary data to be added to the git repository.  

### Building
To build and run this program, navigate to the correct directory. Make a new 'build' directory, cmake in previous directory, then make in the build directory. Then run the program with the shader file names as command line arguments.

Example:
```
mkdir build
cd build
cmake ..
make
./Tutorial cube_shader-v.txt cube_shader-f.txt
```

Note: Make sure that the vertex shader (cube_shader-v.txt) is first in the command line.

### Controls
A key: stop/change spin direction of planet

S key: stop/change spin direction of moon

Left mouse click: stop/change orbit direction of planet

Right mouse click: stop/change orbit direction of moon

### Extra Credit
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


### CMake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary. Later use with CMake and Shader files will be require the copy of a directory where those files are stored (ex. shaders). To do this in the ```add_custom_target``` function place 
```cmake
COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/shaders/ ${CMAKE_CURRENT_BINARY_DIR}/shaders
```

```bash
mkdir build
cd build
cmake ..
make
./Tutorial
```

### Makefile Instructions 
The makefile works as expected and must be updated with new files added in.

```bash
mkdir build
cd build
cp ../makefile .
make
./Tutorial
```

## Ubuntu.cse.unr.edu
OpenGL 3.3 will run on the [ubuntu.cse.unr.edu](https://ubuntu.cse.unr.edu/) website. To do so follow the build instructions, but when running the Tutorial executable use this line to execute.
```bash
/usr/NX/scripts/vgl/vglrun ./Tutorial
```
