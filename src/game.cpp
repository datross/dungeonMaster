#include "game.h"
#include <iostream>

Game::Game(char* _application_path)
    : application_path(_application_path), game_state(STATE_GAMEPLAY) {
//     assets.load("tinymap");
//     assets.save("res/saves/savetinymap.txt");
//     view.setMap(assets.map);
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
}

void Game::gameplay() {
    // TODO ça paraît très temporaire et crade tout ça...
    if(view.get_input() == INPUT_QUIT)
        game_state = STATE_QUIT;
    else if(view.get_input() == INPUT_MOVE_BACKWARD) {
        assets.map.players.begin()->position = assets.map.players.begin()->position + glm::ivec2(0,1);
        std::cout << assets.map.players.begin()->position << std::endl;
    } else if(view.get_input() == INPUT_MOVE_FORWARD) {
        assets.map.players.begin()->position = assets.map.players.begin()->position + glm::ivec2(0,-1);
        std::cout << assets.map.players.begin()->position << std::endl;
    } else if(view.get_input() == INPUT_MOVE_LEFT) {
        assets.map.players.begin()->position = assets.map.players.begin()->position + glm::ivec2(-1,0);
        std::cout << assets.map.players.begin()->position << std::endl;
    } else if(view.get_input() == INPUT_MOVE_RIGHT) {
        assets.map.players.begin()->position = assets.map.players.begin()->position + glm::ivec2(1,0);
        std::cout << assets.map.players.begin()->position << std::endl;
    }
}
