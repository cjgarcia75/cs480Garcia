#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{
    std::string temp = argv[1];
    std::string vsFile = "../shaders/" + temp;
    
    temp = argv[2];
    std::string fsFile = "../shaders/" + temp;
    
    temp = argv[3];
    std::string objFile = "../assets/" + temp;
    
    std::ifstream file;
    
    file.open(vsFile.c_str());
    
    if(!file)
        printf("No vertex file\n");
        
    file.close();
    
    file.open(fsFile.c_str());
    
    if(!file)
        printf("No fragment file\n");
        
    file.close();
    
    file.open(objFile.c_str());
    
    if(!file)
        printf("No obj file\n");
        
    file.close();

  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Tutorial Window Name", 800, 600);
  if(!engine->Initialize(vsFile, fsFile, objFile))
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}
