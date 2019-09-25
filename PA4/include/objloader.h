#include <fstream>
#include <vector>
#include "graphics_headers.h"

class ObjLoader
{
public:
    bool loadOBJ(const char * path, std::vector < glm::vec3 > & out_vertices, std::vector < glm::vec2 > & out_uvs, std::vector < glm::vec3 > & out_normals);
private:
    
};
