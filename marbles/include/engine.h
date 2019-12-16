#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"

class Engine
{
  public:
    Engine(string name, int width, int height, int numOfBalls);
    Engine(string name);
    ~Engine();
    bool Initialize(std::string vsFile, std::string fsFile);
    void Run();
    void Keyboard();
    void mouse();
    unsigned int getDT();
    long long GetCurrentTimeMillis();

  private:
    // Window related variables
    Window *m_window;    
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    int m_numOfBalls;
    SDL_Event m_event;

    Graphics *m_graphics;
    Camera *m_camera;
    //keyboard inputs
    unsigned int m_DT, input, camInput;
    bool w, a, s, d, r, f;
    //mouse inputs
    float xPos, yPos;
    bool firstMouse = true;
    float yaw = -90.0f;
    float pitch = 0.0f;
    long long m_currentTimeMillis;
    bool m_running;
};

#endif // ENGINE_H
