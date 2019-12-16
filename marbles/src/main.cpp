#include <iostream>
#include <cstdlib>
#include "engine.h"


int main(int argc, char **argv)
{
  //read in vertex and fragment shaders
    std::string vsFile = "../shaders/cube_shader-v.txt";
    
    std::string fsFile = "../shaders/cube_shader-f.txt";
    
    std::ifstream file;
    
    file.open(vsFile.c_str());
    
    if(!file)
        printf("No vertex file\n");
        
    file.close();
    
    file.open(fsFile.c_str());
    
    if(!file)
        printf("No fragment file\n");
        
    file.close();

  //read in how many balls to spawn
  int numOfBalls;
  for (int i = 1; i < argc; i++)
  {
    numOfBalls = atoi(argv[i]);
  }

  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Tutorial Window Name", 1400, 1000, numOfBalls);
  if(!engine->Initialize(vsFile, fsFile))
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
