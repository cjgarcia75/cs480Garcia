#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"

class Engine
{
  public:
    Engine(string name, int width, int height);
    Engine(string name);
    ~Engine();
    bool Initialize(std::string vsFile, std::string fsFile);
    void Run();
    void Keyboard();
    void Reset();
    unsigned int getDT();
    long long GetCurrentTimeMillis();
  
  private:
    // Window related variables
    Window *m_window;    
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    SDL_Event m_event;

    float spot = 0.5;
    float spec = 1.0;
    float amb = 1.1;
    int pull_back;
    bool launched;
    bool done;
    int ballCount;
    int score;
    int camInput;
    

    Graphics *m_graphics;
    unsigned int m_DT, input;
    long long m_currentTimeMillis;
    bool m_running;
    
    int ChangePower(int p, bool d);
};

#endif // ENGINE_H
