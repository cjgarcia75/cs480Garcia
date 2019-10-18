
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
  speed = 1;
  prevSpeed = 1;
  
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
        index = system;
      if (ImGui::Button("Sun"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = sun;
      if (ImGui::Button("Mercury"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = mercury;
      if (ImGui::Button("Venus"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = venus;
      if (ImGui::Button("Earth"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = earth;
      if (ImGui::Button("Mars"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = mars;
      if (ImGui::Button("Jupiter"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = jupiter;
      if (ImGui::Button("Saturn"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = saturn;
      if (ImGui::Button("Uranus"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = uranus;
      if (ImGui::Button("Neptune"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = neptune;
      if (ImGui::Button("Pluto"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        index = pluto;
      if (ImGui::Button("1x"))
        speed = 4;
      if (ImGui::Button("100x"))
        speed = 2;
      if (ImGui::Button("1000x"))
        speed = 3;

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();
    }

    // Update and render the graphics
    m_graphics->Update(m_DT, index, speed);
    speed = 1;
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
      case SDLK_1: index = sun;
                   break;
      case SDLK_2: index = mercury;
                   break;
      case SDLK_3: index = venus;
                   break;
      case SDLK_4: index = earth;
                   break;
      case SDLK_5: index = mars;
                   break;
      case SDLK_6: index = jupiter;
                   break;
      case SDLK_7: index = saturn;
                   break;
      case SDLK_8: index = uranus;
                   break;
      case SDLK_9: index = neptune;
                   break;
      case SDLK_0: index = pluto;
                   break;
      case SDLK_r: index = system;
                   break;
      case SDLK_q: speed = 4;
                   break;
      case SDLK_w: speed = 2;
                   break;
      case SDLK_e: speed = 3;
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
