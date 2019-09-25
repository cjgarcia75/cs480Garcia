
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
  
  for(int i = 0; i < num_planets; i++)
  {
    spin[i] = 1;
    rot[i] = 1;
    scale[i] = 1.0;
    s_speed[i] = 1;
    r_speed[i] = 1;
  }

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
    m_graphics->Update(m_DT, spin, rot, scale, s_speed, r_speed);
    m_graphics->Render();

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
      // change spinning direction of planet                  
      case SDLK_a: spin[0] = ChangeDir(spin[0]);
                   break;
      // change spinning direction of moon
      case SDLK_s: spin[1] = ChangeDir(spin[1]);
                   break;
      // increase size of planet
      case SDLK_e: scale[0] = ChangeSize(scale[0], true);
                   break;
      // decrease size of planet
      case SDLK_d: scale[0] = ChangeSize(scale[0], false);
                   break;
      // increase size of moon
      case SDLK_r: scale[1] = ChangeSize(scale[1], true);
                   break;
      // decrease size of moon
      case SDLK_f: scale[1] = ChangeSize(scale[1], false);
                   break;
      // increase speed of planet spin
      case SDLK_t: s_speed[0] = ChangeSpeed(s_speed[0], true);
                   break;
      // decrease speed of planet spin
      case SDLK_g: s_speed[0] = ChangeSpeed(s_speed[0], false);
                   break;
      // increase speed of planet orbit
      case SDLK_y: r_speed[0] = ChangeSpeed(r_speed[0], true);
                   break;
      // decrease speed of planet orbit
      case SDLK_h: r_speed[0] = ChangeSpeed(r_speed[0], false);
                   break;            
      // increase speed of moon spin
      case SDLK_u: s_speed[1] = ChangeSpeed(s_speed[1], true);
                   break;
      // decrease speed of moon spin
      case SDLK_j: s_speed[1] = ChangeSpeed(s_speed[1], false);
                   break;
      // increase speed of moon orbit
      case SDLK_i: r_speed[1] = ChangeSpeed(r_speed[1], true);
                   break;
      // decrease speed of moon orbit
      case SDLK_k: r_speed[1] = ChangeSpeed(r_speed[1], false);
                   break;
    }
  }
  else if (m_event.type == SDL_MOUSEBUTTONDOWN)
  {
    // orbit of cube
    if (m_event.button.button == SDL_BUTTON_LEFT)
    {
      rot[0] = ChangeDir(rot[0]);
    }
    // orbit of moon
    if (m_event.button.button == SDL_BUTTON_RIGHT)
    {
      rot[1] = ChangeDir(rot[1]);
    }
  }
}

int Engine::ChangeDir(int x)
{
  if(x == 1)
    return 0;
  else if(x == 0)
    return -1;
  else if(x == -1)
    return 1;
}

float Engine::ChangeSize(float x, bool add)
{
  if(add)
  {
    x += scale_modifier;
    if(x > 5.0)
      x = 5.0;
  }
  else
  { 
    x -= scale_modifier;
    if(x < 0.01)
      x = 0.01;
  }
  
  return x;
}

int Engine::ChangeSpeed(int x, bool add)
{
  if(add)
  {
    x++;
    if(x > 4)
      x = 4;
  }
  else
  {
    x--;
    if(x < 1)
      x = 1;
  }
  
  return x;
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
