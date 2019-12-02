#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <cmath>
#include "graphics_headers.h"
#include "stb_image.h"
#include "shader.h"

class Object
{
  public:
    Object(std::string objFile, std::string texFile, std::string n, float mass, float inertia, btVector3 pos);
    ~Object();
    void Update(unsigned int dt);
    void Render(GLint& m_modelMatrix, Shader *shader, bool flag);
    
    glm::mat4 GetModel(); 
    
    void SetMass(float mass);
    void SetInertia(float inertia);
    void SetPos(btVector3 pos); 
    
    btCollisionShape* GetShape();
    btRigidBody* GetRigidBody();
    float GetMass();
    float GetInertia();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    
    std::string name;
    float _mass, _inertia;
    btVector3 _pos;
    btCollisionShape* shape;
    btRigidBody* rigidBody;
    
    struct MeshEntry 
    {
      MeshEntry();

      ~MeshEntry();

      void Init(const std::vector<Vertex>& Vertices,
                const std::vector<unsigned int>& Indices);

      GLuint VB;
      GLuint IB;
      unsigned int NumIndices;
      unsigned int MaterialIndex;
    };
    
    struct Texture
    {
      Texture(GLenum TextureTarget, const std::string& FileName);

      bool Load();

      void Bind(GLenum TextureUnit);

      std::string m_fileName;
      GLenum m_textureTarget;
      GLuint m_textureObj;
    };
    
    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;
    
    // model loading
    bool LoadObjFile(std::string objFile, std::string texFile);
    bool InitFromScene(const aiScene* pScene, const std::string& objFile, const std::string texFile);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh, btTriangleMesh *objTriangleMesh);
    bool InitMaterials(const aiScene* pScene, const std::string& objFile, const std::string texFile);
    
    glm::vec3 colors();
};

#endif /* OBJECT_H */
