#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, std::string vsFile, std::string fsFile, std::string objFile);
    void Update(unsigned int dt, int index, int speed);
    void ChangeCamera(int i);
    void Render();
    void ConfigPlanets(std::string file);
    void ConfigRings();
    void ChangeSpeed(Object* p, int speed);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLuint sampler;

    const static int numPlanets = 32;
    std::string texFiles[numPlanets];
    Object *planets[numPlanets];
    Object *rings;
    
    const static int system = -1,
                     sun = 1,
                     mercury = 2,
                     venus = 3,
                     earth = 4,
                     mars = 6, 
                     jupiter = 9,
                     saturn = 14,
                     uranus = 22, 
                     neptune = 28, 
                     pluto = 30;
};

#endif /* GRAPHICS_H */
