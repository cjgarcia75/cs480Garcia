#include "object.h"

Object::Object(std::string objFile)
{  
  /*
    # Blender File for a Cube
    o Cube
    v 1.000000 -1.000000 -1.000000
    v 1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 -1.000000
    v 1.000000 1.000000 -0.999999
    v 0.999999 1.000000 1.000001
    v -1.000000 1.000000 1.000000
    v -1.000000 1.000000 -1.000000
    s off
    f 2 3 4
    f 8 7 6
    f 1 5 6
    f 2 6 7
    f 7 8 4
    f 1 4 8
    f 1 2 4
    f 5 8 6
    f 2 1 6
    f 3 2 7
    f 3 7 4
    f 5 1 8
  */

  Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  };

  Indices = {
    2, 3, 4,
    8, 7, 6,
    1, 5, 6,
    2, 6, 7,
    7, 8, 4,
    1, 4, 8,
    1, 2, 4,
    5, 8, 6,
    2, 1, 6,
    3, 2, 7,
    3, 7, 4,
    5, 1, 8
  };

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }
  
  loader = new ObjLoader;
  const char* file = objFile.c_str();
  
  if(!loader->loadOBJ(file , vertices, uvs, normals))
  {
    printf("Could not load object.\n");
    return;
  }

  angle = 0.0f;
  moon = false;

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt)
{
  angle = SetAngle(dt, _rot, angle, rot_speed);
  angle1 = SetAngle(dt, _spin, angle1, spin_speed);
  
  if(moon)
  {
    model = glm::translate(parent, glm::vec3((4 * cos(angle)), 0.0, (4 * sin(angle))));
  }
  else
  {
    model = glm::translate(glm::mat4(1.0f), glm::vec3((3 * cos(angle)), 0.0, (3 * sin(angle))));
    tranModel = model;
  }
  model = glm::rotate(model, (angle1), glm::vec3(0.0, 1.0, 0.0));
  model = glm::scale(model, glm::vec3(_scale, _scale, _scale));
}

float Object::SetAngle(unsigned int dt, int x, float angle, int speed)
{
  if(x == 1)
    angle += dt * M_PI/1000 * speed;
  else if(x == -1)
    angle -= dt * M_PI/1000 * speed;
  else
    angle = angle;
    
  return angle;
}

glm::mat4 Object::GetModel()
{
  return model;
}

glm::mat4 Object::GetTran()
{
  return tranModel;
} 

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void Object::SetSpin(int spin)
{
  _spin = spin;
}

void Object::SetRot(int rot)
{
  _rot = rot;
}

void Object::SetSSpeed(int speed)
{
  spin_speed = speed;
}

void Object::SetRSpeed(int speed)
{
  rot_speed = speed;
}

void Object::SetScale(float scale)
{
  _scale = scale;
}

void Object::SetParent(glm::mat4 model)
{
  parent = model;
}
void Object::SetMoon(bool is_moon)
{
  moon = is_moon;
}
