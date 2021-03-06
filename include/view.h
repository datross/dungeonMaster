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

    /* for events */
    void update();

    /* draw actual buffer */
    void render(Game_state& game_state);

    Player_input get_input();

	void initTextures();

    /* Rendering function depends on the game state */
    void mainMenu(Game_state& game_state);
    void renderGame(Game_state& game_state);
    void HUD(Game_state& game_state);

    void setAssets(Assets&);

    void updateEvent();

    /* update every camera */
    void reshape(unsigned w, unsigned h);

    /* about window */
    SDL_Window* window;
    SDL_GLContext context_gl;
    SDL_Event event;

    /* input events, already interpreted for the game */
    Player_input player_input;
    glm::ivec2 mouse_pos;
    glm::ivec2 mouse_pos_rel;

    unsigned window_width,
             window_height;

    /* about game rendering */
    /* everything is stored in assets */
    /* not a shared_ptr because no need to delete */
    Assets * assets_ptr;
};

#endif
