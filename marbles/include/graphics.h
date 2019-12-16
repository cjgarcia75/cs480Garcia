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
    bool Initialize(int width, int height, std::string vsFile, std::string fsFile, int numOfBalls);
    void Update(unsigned int dt, unsigned int input);
    void Render(bool w, bool a, bool s, bool d, bool r, bool f, unsigned int dt, double xPos, double yPos, unsigned int camInput);
    void SwitchShader();
    
    bool BulletInit();
    bool switchShader;
    btDiscreteDynamicsWorld* GetDynamicsWorld() const;

    void initBalls();
    void SetLights(int ballNum, glm::vec3 tempVec[], float lc[3], float a[3], float d[3], float s[3], float c, float l, float q);

    void setWeight(float wieght);
    float getWeight();

  private:
    std::string ErrorString(GLenum error);
    int g_numOfBalls;

    Camera *m_camera;
    Shader *m_shader;
    Shader *otherShader;

    bool flag = true;
    float m_ballWeight;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint otherProjectionMatrix;
    GLint otherViewMatrix;
    GLint otherModelMatrix;
    GLuint sampler;

    Object *balls[100];
    Object *bumper1;
    Object *bumper2;
    Object *bumper3;
    Object *obsticle1;
    Object *ramp2;
    Object *rotator;
    Object *slide1;
    Object *funnel;
    Object *tunnel;
    Object *tunnelJump;
    Object *twisty;
    Object *bucket;
    
    btVector3 *ballPositions;

    glm::mat4 tempMat;
    glm::vec3 tempVec[105];
    
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* broadphase;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
};

#endif /* GRAPHICS_H */
