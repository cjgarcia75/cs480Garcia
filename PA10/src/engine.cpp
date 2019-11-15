
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
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, vsFile, fsFile))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();
  
  input = 0;
  camInput = 0;
  launched = false;
  done = true;
  pull_back = 0;
  ballCount = 3;
  score = 0;
  
  
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
    
    //Menu. 
    {
      ImGui::Begin("Pinball");
      
      if(ballCount == 0)
        ImGui::Text("GAME OVER");
        
      ImGui::Text("Score: %d", score);
      
      ImGui::Text("Ball Count: %d", ballCount);

      ImGui::Text("Pull back power: %d", pull_back);

      ImGui::End();
    }

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
    }

    // Update and render the graphics
    m_graphics->Update(m_DT, input, pull_back, launched, done, ballCount, camInput);
    m_graphics->Render(spot, amb, spec);
    
    //input = 0;
    camInput = 0;
    launched = false;
    
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
      case SDLK_RIGHT: input = 1; 
                        break;     
      case SDLK_LEFT: input = 3; 
                        break;
      case SDLK_w: camInput = 1;
                        break;
      case SDLK_s: camInput = 2;
                        break;
      case SDLK_a: camInput = 3;
                        break;
      case SDLK_d: camInput = 4;
                        break;
      case SDLK_n: if(ballCount == 0)
                     Reset();
                        break;
      case SDLK_l: m_graphics->SwitchShader();
                        break;    
      case SDLK_DOWN: pull_back = ChangePower(pull_back, true);
                        break;
      case SDLK_UP: pull_back = ChangePower(pull_back, false);
                        break;    
      case SDLK_SPACE: if(done)
                       {
                         launched = true;
                         done = false;
                         score += 50;
                       }
                        break;                           
    }
    if (m_event.type == SDL_KEYDOWN)
    {
      switch(m_event.key.keysym.sym)
      {
        case SDLK_f: spot -= 1.0;
                        break;
        case SDLK_r: spot += 1.0;
                        break;
        case SDLK_t: amb += 0.1;
                        break;
        case SDLK_g: amb -= 0.1;
                        break;
        case SDLK_y: spec += 0.1;
                        break;
        case SDLK_h: spec -= 0.1;
                        break;                                                                     
      }
    }
  }
  else if(m_event.type == SDL_KEYUP)
  {
    switch(m_event.key.keysym.sym)
    {
      case SDLK_RIGHT: input = 0;
                       break;
      case SDLK_LEFT: input = 2;
                      break;
    }
  }
}

int Engine::ChangePower(int p, bool d)
{
  if(d)
  {
    if(pull_back == 4)
      return 4;
    
    pull_back++;
    return pull_back;
  }
  else
  {
    if(pull_back == 0)
      return 0;
      
    pull_back--;
    return pull_back;
  }
}

void Engine::Reset()
{
  input = 0;
  launched = false;
  done = true;
  pull_back = 0;
  ballCount = 4;
  score = 0;
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
