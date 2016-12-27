#ifndef VIEW_H
#define VIEW_H

#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "mesh.h"
#include "camera.h"
#include "assets.h"

#include "gui.h"

enum Player_input {
    INPUT_NONE,
    INPUT_QUIT,
    INPUT_MOVE_LEFT,
    INPUT_MOVE_RIGHT,
    INPUT_MOVE_FORWARD,
    INPUT_MOVE_BACKWARD
};

enum Game_state {
    STATE_MENU,
    STATE_GAMEPLAY,
    STATE_QUIT
};

class View {
public:
    View();
    ~View();
    void update();
    void render(Game_state& game_state);

    Player_input get_input();

	void mainMenu(Game_state& game_state);

    void setAssets(Assets&);
private:
    void updateEvent();
    void reshape(unsigned w, unsigned h);

    // about window
    SDL_Window* window;
    SDL_GLContext context_gl;
    SDL_Event event;
    Player_input player_input;

    unsigned window_width,
             window_height;

    // about game rendering
    // everything is stored in assets
    std::shared_ptr<Assets> assets_ptr;
};

#endif
