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
    void Update(unsigned int dt, int index);
    void ChangeCamera(int i);
    void Render();
    void ConfigPlanets(std::string file);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLuint sampler;

    const static int numPlanets = 12;
    std::string texFiles[numPlanets];
    Object *planets[numPlanets];
};

#endif /* GRAPHICS_H */
