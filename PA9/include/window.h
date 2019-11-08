#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>
#include <string>
using namespace std;

class Window
{
  public:
    Window();
    ~Window();
    bool Initialize(const string &name, int* width, int* height);
    void Swap();
    //IMGUI_CHECKVERSION();
    //CreateContext();
    SDL_Window* gWindow;
    SDL_GLContext gContext;

  private:
    //ImFontAtlas* atlas = new ImFontAtlas();
    //ImGuiContext* icontext = ImGui::CreateContext(atlas);
};

#endif /* WINDOW_H */
