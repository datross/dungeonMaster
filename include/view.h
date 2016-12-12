#ifndef VIEW_H
#define VIEW_H

#include <SDL2/SDL.h>
#include <GL/glew.h>

enum Player_input {
    INPUT_NONE,
    INPUT_QUIT 
};

class View {
public:
    View();
    ~View();
    void update();
    void render();
    
    Player_input get_input();
private:
    SDL_Window* window;
    SDL_GLContext context_gl;
    SDL_Event event;
    Player_input player_input;
    
    unsigned window_width,
             window_height;
};

#endif