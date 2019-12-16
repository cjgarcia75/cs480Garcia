#include "engine.h"

Engine::Engine(string name, int width, int height, int numOfBalls)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
  m_numOfBalls = numOfBalls;
  input = 0;
  camInput = 3;
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

bool Engine::Initialize(std::string vsFile, std::string fsFile)
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
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, vsFile, fsFile, m_numOfBalls))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();
  
  input = 0;
  
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

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
    }

    // Update and render the graphics
    m_graphics->Update(m_DT, input);
    m_graphics->Render(w, a, s, d, r, f, m_DT, m_event.motion.x, m_event.motion.y, camInput);
    

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
      case SDLK_w: w = true; 
                        break;     
      case SDLK_s: s = true; 
                        break;
      case SDLK_a: a = true; 
                        break;
      case SDLK_d: d = true; 
                        break;
      case SDLK_UP: f = true; 
                        break;
      case SDLK_DOWN: r = true; 
                        break;
      case SDLK_l: m_graphics->SwitchShader();
                        break;
      case SDLK_1: camInput = 3; 
                        break;
      case SDLK_2: camInput = 4; 
                        break;
      case SDLK_3: camInput = 5; 
                        break;
      case SDLK_4: camInput = 6; 
                        break;
      case SDLK_5: camInput = 7; 
                        break;
      case SDLK_6: camInput = 8; 
                        break;
      case SDLK_7: camInput = 9; 
                        break;
      case SDLK_8: camInput = 10; 
                        break;
      case SDLK_9: camInput = 11; 
                        break;
      case SDLK_0: camInput = 12; 
                        break;
    }
  }
  else if (m_event.type == SDL_KEYUP)
  {
    // handle key down events here
    switch(m_event.key.keysym.sym)
    {
      // end program
      case SDLK_ESCAPE: m_running = false; 
                        break;
      case SDLK_w: w = false; 
                        break;     
      case SDLK_s: s = false; 
                        break;
      case SDLK_a: a = false; 
                        break;
      case SDLK_d: d = false; 
                        break;
      case SDLK_UP: f = false; 
                        break;
      case SDLK_DOWN: r = false; 
                        break;
      case SDLK_l: m_graphics->SwitchShader();
                        break;
    }
  }
}

void Engine::mouse()
{
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
