#include "camera.h"

Camera::Camera()
{
  x = radius * sin(angle);
  z = radius * cos(angle);
  y = 12.0;
  cameraPos = glm::vec3(x, y, -z);
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
void Camera::Update(bool w, bool a, bool s, bool d, bool r, bool f, glm::vec3 ballPos)
{
  if (w == true)
  {
    if(y < 30.0f)
      y += 0.2f;
  }
  if (s == true)
  {
    if(y > 10.0f)
      y -= 0.2f;
  }
  if (f == true)
  {
    if(fov > 44.3f)
      fov -= 0.01f;
  }
  if (a == true)
  {
    angle += 0.01f;
    x = radius * sin(angle);
    z = radius * cos(angle);
  }
  if (r == true)
  {
    if(fov < 45.0f)
      fov += 0.01f;
  }
  if (d == true)
  {
    angle -= 0.01f;
    x = radius * sin(angle);
    z = radius * cos(angle);
  }
  cameraPos = glm::vec3(ballPos.x + x, ballPos.y + y, ballPos.z - z);
  view = glm::lookAt(cameraPos, ballPos, cameraUp);
  projection = glm::perspective(fov, width/height, 0.01f, 350.0f);
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
