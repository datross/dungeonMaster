#include "game.h"
#include <iostream>

Game::Game(char* _application_path)
    : application_path(_application_path), game_state(STATE_MENU) {
}

Game::~Game() {
    
}

int Game::run() {
    while(game_state != STATE_QUIT) {
        
        view.update();
        
        if(game_state == STATE_MENU) {
            menu();
        } else if(game_state == STATE_GAMEPLAY) {
            gameplay();
        }
        
        view.render();
    }
    
    return 0;
}

void Game::menu() {
    if(view.get_input() == INPUT_QUIT)
        game_state = STATE_QUIT;
}

void Game::gameplay() {
}
