#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "stb_image.h"

class Object
{
  public:
    Object(std::string objFile);
    ~Object();
    void Update(unsigned int dt);
    void Render();
    
    // moon and change variables
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
//    GLuint VB;
//    GLuint IB;
    
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
    
    // model loading
    bool LoadObjFile(std::string objFile);
    bool InitFromScene(const aiScene* pScene, const std::string& objFile);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
    bool InitMaterials(const aiScene* pScene, const std::string& objFile);
    
    float SetAngle(unsigned int dt, int x, float angle, int speed);
    glm::vec3 colors();
};

#endif /* OBJECT_H */
