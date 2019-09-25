#include "objloader.h"

bool ObjLoader::loadOBJ(const char * path, std::vector < glm::vec3 > & out_vertices, std::vector < glm::vec2 > & out_uvs, std::vector < glm::vec3 > & out_normals)
{
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;
    unsigned int vertexI,
                 uvI,
                 normalI;
    
    FILE * ifile = fopen(path, "r");
    if( ifile == NULL )
    {
        printf("Cannot open obj file.\n");
        return false;
    }
    
    while(true)
    {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(ifile, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader
        if (strcmp(lineHeader, "v") == 0)
        {
            glm::vec3 vertex;
            fscanf(ifile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
            
            //std::cout << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            glm::vec2 uv;
            fscanf(ifile, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
            
            //std::cout << uv.x << " " << uv.y << "\n";
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            glm::vec3 normal;
            fscanf(ifile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
            
            //std::cout << normal.x << " " << normal.y << " " << normal.z << "\n";
        }
        else if ( strcmp( lineHeader, "f" ) == 0 )
        {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(ifile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            
            if (matches != 9)
            {
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }
        
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }
    
    for(unsigned int i=0; i<vertexIndices.size(); i++)
    {
        vertexI = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexI - 1];
        out_vertices.push_back(vertex);
        
        uvI = uvIndices[i];
        glm::vec2 uv = temp_uvs[uvI - 1];
        out_uvs.push_back(uv);
        
        normalI = normalIndices[i];
        glm::vec3 norm = temp_normals[normalI - 1];
        out_normals.push_back(norm);
    }
    
    return true;
}
