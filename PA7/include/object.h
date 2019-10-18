#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <cmath>
#include "graphics_headers.h"
#include "stb_image.h"

class Object
{
  public:
    Object(std::string objFile, std::string texFile);
    ~Object();
    void Update(unsigned int dt);
    void Render();
    
    // moon and change variables
    void SetName(std::string n);
    void SetSpin(int spin);
    void SetRot(int rot);
    void SetSSpeed(float speed);
    void SetRSpeed(float speed);
    void SetScale(float scale);
    void SetParent(glm::mat4 model);
    void SetMoon(bool is_moon);
    void SetRing(bool is_ring);
    void SetRadius(float rad);
    void SetTilt(float t);
    void SetOrbital(float o);
    
    void SetSpeed1();
    void SetSpeed2();
    void SetSpeed3();
    
    std::string GetName();
    bool GetMoon();
    bool GetRing();
    float GetSSpeed();
    float GetRSpeed();
    glm::mat4 GetTran();
    glm::mat4 GetModel();    

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    
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
    std::string name;
    float angle;
    float angle1;
    bool moon;
    bool ring;
    int _spin;
    int _rot;
    float spin_speed;
    float rot_speed;
    float _scale;
    float radius;
    float tilt;
    float orbital;
    float ss1, ss2, ss3, rs1, rs2, rs3;
    
    // model loading
    bool LoadObjFile(std::string objFile, std::string texFile);
    bool InitFromScene(const aiScene* pScene, const std::string& objFile, const std::string texFile);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
    bool InitMaterials(const aiScene* pScene, const std::string& objFile, const std::string texFile);
    
    float SetAngle(unsigned int dt, int x, float angle, float speed);
    glm::vec3 colors();
};

#endif /* OBJECT_H */
