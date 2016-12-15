#ifndef VIEW_H
#define VIEW_H

#include <memory>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "mesh.h"
#include "camera.h"
#include "map.h"

enum Player_input {
    INPUT_NONE,
    INPUT_QUIT,
    INPUT_MOVE_LEFT,
    INPUT_MOVE_RIGHT,
    INPUT_MOVE_FORWARD,
    INPUT_MOVE_BACKWARD
};

class View {
public:
    View();
    ~View();
    void update();
    void render();
    
    Player_input get_input();
    
    void setMap(Map&);
private:
    void updateEvent();
    
    SDL_Window* window;
    SDL_GLContext context_gl;
    SDL_Event event;
    Player_input player_input;
    
    unsigned window_width,
             window_height;
             
    std::shared_ptr<Map> map_ptr;
};

#endif