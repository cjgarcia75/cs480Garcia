#include "camera.h"

Camera::Camera()
{
  x = radius * sin(angle);
  z = radius * cos(angle);
  cameraPos = glm::vec3(x, 12.0, -z);
}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to be more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  //Positive Y is up
  width = float(w);
  height = float(h);
  view = glm::lookAt( cameraPos, cameraPos + cameraFront, cameraUp);
  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 350.0f); //Distance to the far plane, 
  return true;
}

//Moves the camera
void Camera::Update(bool w, bool a, bool s, bool d, unsigned int dt, glm::vec3 ballPos)
{
  if (w == true)
  {
    if(fov > 30.0f)
      fov -= 0.01f;
    //if(fov <= 40.0f)
      //fov = 40.0f;
  }
  if (a == true)
  {
    angle += 0.01f;
    x = radius * sin(angle);
    //y = radius * sin(angle) * sin(angle1);
    z = radius * cos(angle);
    cameraPos = glm::vec3(x, ballPos.y + 12.0, -z);
    //cameraPos += cameraSpeed * cameraFront;
    //view = glm::lookAt(glm::vec3(x, cameraPos.y, z),  ballPos + cameraFront, cameraUp);
    //cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  }
  if (s == true)
  {
    if(fov < 45.0f)
      fov += 0.01f;
    //if(fov >= 45.0f)
      //fov = 45.0f;
  }
  if (d == true)
  {
    angle -= 0.01f;
    x = radius * sin(angle);
    //y = radius * sin(angle) * sin(angle1);
    z = radius * cos(angle);
    cameraPos = glm::vec3(x, ballPos.y + 12.0, -z);
    //cameraPos += cameraSpeed * cameraFront;
    //view = glm::lookAt(glm::vec3(x, cameraPos.y, z),  ballPos + cameraFront, cameraUp);
    //cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  }
  view = glm::lookAt(cameraPos, ballPos, cameraUp);
  projection = glm::perspective(fov, width/height, 0.01f, 350.0f);
  //view = glm::lookAt( cameraPos,  ballPos + cameraFront, cameraUp);
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

void Camera::mouseInput(double xPos, double yPos)
{
    //get the offset for when the mouse first enters the window
    /*if (firstMouse)
    {
      lastX = xPos;
      lastY = yPos;
      firstMouse = false;
      std::cout << "hello? " << std::endl;
    }*/
    xoffset = xPos - lastX;
    yoffset = yPos- lastY;
    lastX = xPos;
    lastY = yPos;

    sensitivity = 0.01f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
    {
      pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
      pitch = -89.0f;
    }

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    view = glm::lookAt( cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::GetView()
{
  //view = glm::lookAt( cameraPos, cameraPos + cameraFront, cameraUp);
  return view;
}
