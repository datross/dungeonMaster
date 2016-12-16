#include "game.h"
#include <iostream>

Game::Game(char* _application_path)
    : application_path(_application_path), game_state(STATE_GAMEPLAY) {
    map.load("tinymap");
    map.save("res/saves/saveTest.txt");
    //view.setMap(map);
    
        
        
    std::cout << "Enemies" << std::endl;
    std::list<Enemy>::iterator it = map.characters.begin();
    for (; it != map.characters.end(); it++) {
        it->print();
        it->reach(glm::ivec2(0,0), &map);
    }
    
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
    if(view.get_input() == INPUT_QUIT)
        game_state = STATE_QUIT;
    else if(view.get_input() == INPUT_MOVE_BACKWARD) {
        map.players.begin()->position = map.players.begin()->position + glm::ivec2(0,1);
        std::cout << map.players.begin()->position << std::endl;
    } else if(view.get_input() == INPUT_MOVE_FORWARD) {
        map.players.begin()->position = map.players.begin()->position + glm::ivec2(0,-1);
        std::cout << map.players.begin()->position << std::endl;
    } else if(view.get_input() == INPUT_MOVE_LEFT) {
        map.players.begin()->position = map.players.begin()->position + glm::ivec2(-1,0);
        std::cout << map.players.begin()->position << std::endl;
    } else if(view.get_input() == INPUT_MOVE_RIGHT) {
        map.players.begin()->position = map.players.begin()->position + glm::ivec2(1,0);
        std::cout << map.players.begin()->position << std::endl;
    }
}
