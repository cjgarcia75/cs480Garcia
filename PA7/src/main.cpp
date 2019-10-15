#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{
    std::string vsFile = "../shaders/cube_shader-v.txt";
    
    std::string fsFile = "../shaders/cube_shader-f.txt";
    
    std::string objFile = "../assets/sphere.obj";
    
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
  Engine *engine = new Engine("Tutorial Window Name", 1400, 1000);
  if(!engine->Initialize(vsFile, fsFile, objFile))
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  engine->Run();

  //delete menu
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  delete engine;
  engine = NULL;
  return 0;
}
