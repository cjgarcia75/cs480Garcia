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
    void Update(unsigned int dt, unsigned int input, int pull_back, bool launched);
    void Render(float spot, float amb, float spec);
    void SwitchShader();
    //void CheckLights(Object* obj, int num, int pull_back, bool launched);
    
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

    Object *resetBox;
    Object *ball;
    Object *board;
    Object *sBumper1;
    Object *sBumper2;
    Object *sBumper3;
    Object *flipper1;
    Object *flipper2; //do i need to create two of these?
    Object *boarder1; // to light up the board
    Object *boarder2;
    Object *boarder3;
    Object *boarder4;
    Object *boarder5;
    Object *boarder6;
    Object *pLight1;

    glm::mat4 tempMat;
    glm::vec3 tempVec;
    
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* broadphase;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
};

#endif /* GRAPHICS_H */
