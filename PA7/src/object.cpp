#define STB_IMAGE_IMPLEMENTATION
#include "object.h"

Object::MeshEntry::MeshEntry()
{
  NumIndices  = 0;
};

Object::MeshEntry::~MeshEntry()
{
  glDeleteBuffers(1, &VB);
  glDeleteBuffers(1, &IB);
}

void Object::MeshEntry::Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices)
{
  NumIndices = Indices.size();

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);
}

Object::Object(std::string objFile, std::string texFile)
{  
  angle = 0.0f;
  moon = false;
  ring = false;
  
  if(!LoadObjFile(objFile, texFile))
  {
    std::cout << "Could not open object file\n";
  }
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
  
  float x = radius * sin(angle) * cos(glm::radians(orbital)),
        y = radius * sin(angle) * sin(glm::radians(orbital)),
        z = radius * cos(angle);
  
  if(ring)
  {
    model = glm::translate(parent, glm::vec3(0.0, 0.0, 0.0));
  }
  else if(moon)
  {
    model = glm::translate(parent, glm::vec3(x, y, z));
    model = glm::rotate(model, glm::radians(100.0f), glm::vec3(0.0, 1.0, 0.0));
  }
  else
  {
    model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    
    tranModel = model;
  }
  model = glm::rotate(model, glm::radians(tilt), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, (angle1), glm::vec3(0.0, 1.0, 0.0));
  model = glm::scale(model, glm::vec3(_scale, _scale, _scale));
}

float Object::SetAngle(unsigned int dt, int x, float angle, float speed)
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
  glEnableVertexAttribArray(2);

  for (unsigned int i = 0 ; i < m_Entries.size() ; i++) 
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

    const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

    if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) 
    {
      m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
    }

    glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

bool Object::LoadObjFile(std::string objFile, std::string texFile)
{
  bool Ret = false;
  Assimp::Importer Importer;

  const aiScene* pScene = Importer.ReadFile(objFile, aiProcess_Triangulate);
    
  if (pScene) 
  {
    Ret = InitFromScene(pScene, objFile, texFile);
  }
  else 
  {
    printf("Error parsing '%s': '%s'\n", objFile.c_str(), Importer.GetErrorString());
  }

  return Ret;
}

bool Object::InitFromScene(const aiScene* pScene, const std::string& objFile, const std::string texFile)
{  
  m_Entries.resize(pScene->mNumMeshes);
  m_Textures.resize(pScene->mNumMaterials);

  // Initialize the meshes in the scene one by one
  for (unsigned int i = 0 ; i < m_Entries.size() ; i++) 
  {
    const aiMesh* paiMesh = pScene->mMeshes[i];
    InitMesh(i, paiMesh);
  }

  return InitMaterials(pScene, objFile, texFile);
}

void Object::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
  m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

  const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

  for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) 
  {
    const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
    const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

    Vertex v(glm::vec3(pPos->x, pPos->y, pPos->z),
      glm::vec3(0.0, 0.0, 0.0),
      glm::vec2(pTexCoord->x, pTexCoord->y));

    Vertices.push_back(v);
  }

  for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) 
  {
    const aiFace& Face = paiMesh->mFaces[i];
    assert(Face.mNumIndices == 3);
    Indices.push_back(Face.mIndices[0]);
    Indices.push_back(Face.mIndices[1]);
    Indices.push_back(Face.mIndices[2]);
  }

  m_Entries[Index].Init(Vertices, Indices);
}

bool Object::InitMaterials(const aiScene* pScene, const std::string& objFile, const std::string texFile)
{
  // Extract the directory part from the file name
  std::string::size_type SlashIndex = objFile.find_last_of("/");
  std::string Dir;

  if (SlashIndex == std::string::npos) 
  {
    Dir = ".";
  }
  else if (SlashIndex == 0) 
  {
    Dir = "/";
  }
  else 
  {
    Dir = objFile.substr(0, SlashIndex);
  }

  bool Ret = true;

  // Initialize the materials
  for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) 
  {
    const aiMaterial* pMaterial = pScene->mMaterials[i];

    m_Textures[i] = NULL;

      if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) 
      {
        aiString Path;

        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
        {
          std::string FullPath = Dir + "/" + Path.data;
          m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

        if (!m_Textures[i]->Load()) 
        {
          printf("Error loading texture '%s'\n", FullPath.c_str());
          delete m_Textures[i];
          m_Textures[i] = NULL;
          Ret = false;
        }
        else 
        {
          printf("Loaded texture '%s'\n", FullPath.c_str());
        }
      }
    }

        // Load a white texture in case the model does not include its own texture
    if (!m_Textures[i]) 
    {
      m_Textures[i] = new Texture(GL_TEXTURE_2D, texFile);

      Ret = m_Textures[i]->Load();
    }
  }

  return Ret;
}

Object::Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
  m_textureTarget = TextureTarget;
  m_fileName      = FileName;
}


bool Object::Texture::Load()
{
  unsigned char* data;
  int width, height, nrChannels;
  
  data = stbi_load(m_fileName.c_str(), &width, &height, &nrChannels, 4);
  if(!data)
    return false;

  glGenTextures(1, &m_textureObj);
  glBindTexture(GL_TEXTURE_2D, m_textureObj);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  delete data;
  
  return true;
}

void Object::Texture::Bind(GLenum TextureUnit)
{
  glActiveTexture(TextureUnit);
  glBindTexture(m_textureTarget, m_textureObj);
}

glm::vec3 Object::colors()
{
  float rgb[3];
  
  for(int i = 0; i < 3; i++)
  {
    rgb[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  }
  
  return glm::vec3(rgb[0], rgb[1], rgb[2]);
}

bool Object::GetMoon()
{
  return moon;
}

bool Object::GetRing()
{
  return ring;
}

std::string Object::GetName()
{
  return name;
}

float Object::GetSSpeed()
{
  return spin_speed;
}

float Object::GetRSpeed()
{
  return rot_speed;
}

void Object::SetName(std::string n)
{
  name = n;
}

void Object::SetSpin(int spin)
{
  _spin = spin;
}

void Object::SetRot(int rot)
{
  _rot = rot;
}

void Object::SetSSpeed(float speed)
{
  spin_speed = speed;
  ss1 = speed;
  ss2 = ss1 * 100.0;
  ss3 = ss2 * 10.0;
}

void Object::SetRSpeed(float speed)
{
  rot_speed = speed;
  rs1 = speed;
  rs2 = rs1 * 100.0;
  rs3 = rs2 * 10.0;
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

void Object::SetRing(bool is_ring)
{
  ring = is_ring;
}

void Object::SetRadius(float rad)
{
  radius = rad;
}

void Object::SetTilt(float t)
{
  tilt = t;
}

void Object::SetOrbital(float o)
{
  orbital = o;
}

void Object::SetSpeed1()
{
  spin_speed = ss1;
  rot_speed = rs1;
}

void Object::SetSpeed2()
{
  spin_speed = ss2;
  rot_speed = rs2;
}

void Object::SetSpeed3()
{
  spin_speed = ss3;
  rot_speed = rs3;
}
