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
    
    const static int num_planets = 2;
    int spin[num_planets];
    int rot[num_planets];
    int s_speed[num_planets];
    int r_speed[num_planets];
    float scale[num_planets];
    float scale_modifier = 0.02;
    
    int ChangeDir(int x);
    float ChangeSize(float x, bool add);
    int ChangeSpeed(int x, bool add);
    
};

#endif // ENGINE_H
