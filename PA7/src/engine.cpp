
#include "engine.h"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize(std::string vsFile, std::string fsFile, std::string objFile)
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, vsFile, fsFile, objFile))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();
  
  index = -1;
  
  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_window->gWindow);
    ImGui::NewFrame();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
    }

    //Menu. Adds a button and changes index for each planet
    {
      ImGui::Begin("Solar System");                          // Create a window called "Hello, world!" and append into it.

      ImGui::Text("Select a planet to focus on");               // Display some text (you can use a format strings too)
     
      // Edit 3 floats representing a color
      if (ImGui::Button("Solar System"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = -1;
      if (ImGui::Button("Sun"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = 1;
      if (ImGui::Button("Mercury"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = 2;
      if (ImGui::Button("Venus"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = 3;
      if (ImGui::Button("Earth"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = 4;
      if (ImGui::Button("Mars"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = 6;
      if (ImGui::Button("Jupiter"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = 7;
      if (ImGui::Button("Saturn"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = 8;
      if (ImGui::Button("Uranus"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = 9;
      if (ImGui::Button("Neptune"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = 10;
      if (ImGui::Button("Pluto"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = 11;

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();
    }

    // Update and render the graphics
    m_graphics->Update(m_DT, index);
    m_graphics->Render();

    //menu render each frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    switch(m_event.key.keysym.sym)
    {
      // end program
      case SDLK_ESCAPE: m_running = false; 
                        break;
      // change camera view                 
      case SDLK_1: index = 1;
                   break;
      case SDLK_2: index = 2;
                   break;
      case SDLK_3: index = 3;
                   break;
      case SDLK_4: index = 4;
                   break;
      case SDLK_5: index = 6;
                   break;
      case SDLK_6: index = 7;
                   break;
      case SDLK_7: index = 8;
                   break;
      case SDLK_8: index = 9;
                   break;
      case SDLK_9: index = 10;
                   break;
      case SDLK_0: index = 11;
                   break;
      case SDLK_r: index = -1;
                   break;
    }
  }
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
