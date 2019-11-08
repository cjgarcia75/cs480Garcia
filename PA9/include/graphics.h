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
    bool Initialize(int width, int height, std::string vsFile, std::string fsFile);
    void Update(unsigned int dt, unsigned int input);
    void Render();
    void SwitchShader();
    
    bool BulletInit();
    bool switchShader;
    btDiscreteDynamicsWorld* GetDynamicsWorld() const;

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;
    Shader *otherShader;

    bool flag = true;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint otherProjectionMatrix;
    GLint otherViewMatrix;
    GLint otherModelMatrix;
    GLuint sampler;

    Object *ball;
    Object *board;
    Object *cube;
    Object *cylinder;
    
    glm::mat4 tempMat;
    glm::vec3 tempVec;
    
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* broadphase;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
};

#endif /* GRAPHICS_H */
