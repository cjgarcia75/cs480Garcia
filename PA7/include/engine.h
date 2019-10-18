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
    bool Initialize(std::string vsFile, std::string fsFile, std::string objFile);
    void Run();
    void Keyboard();
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

    Graphics *m_graphics;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;
    int index;
    int speed;
    int prevSpeed;
    
    const static int system = -1,
                     sun = 1,
                     mercury = 2,
                     venus = 3,
                     earth = 4,
                     mars = 6, 
                     jupiter = 9,
                     saturn = 14,
                     uranus = 22, 
                     neptune = 28, 
                     pluto = 30;
};

#endif // ENGINE_H
