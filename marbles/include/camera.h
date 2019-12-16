#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    void Update(bool w, bool a, bool s, bool d, bool r, bool f, glm::vec3 ballPos);
    void mouseInput(double xPos, double yPos);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
  
  private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront = glm::vec3(0.0, 0.0, 2.0);
    glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
    float fov = 45.0f;
    float width, height;
    float x, y, z;
    float radius = 30.0f;
    float angle = 0.0f;
    float angle1 = 0.0f;
    float lastX = 700;
    float lastY = 500;
    bool firstMouse = true;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float xoffset;
    float yoffset;
    float sensitivity = 0.01;
    glm::vec3 front;
};

#endif /* CAMERA_H */
