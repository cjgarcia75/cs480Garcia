#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();
    void Update(unsigned int dt);
    void Render();
    
    void SetSpin(int spin);
    void SetRot(int rot);
    void SetSSpeed(int speed);
    void SetRSpeed(int speed);
    void SetScale(float scale);
    void SetParent(glm::mat4 model);
    void SetMoon(bool is_moon);
    glm::mat4 GetTran();

    glm::mat4 GetModel();    

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    
    glm::mat4 parent;
    glm::mat4 tranModel;
    float angle;
    float angle1;
    bool moon;
    int _spin;
    int _rot;
    int spin_speed;
    int rot_speed;
    float _scale;
    
    float SetAngle(unsigned int dt, int x, float angle, int speed);
};

#endif /* OBJECT_H */
