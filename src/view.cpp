#include <iostream>
#include "view.h"


View::View()
    : player_input(INPUT_NONE) {
    // Initialisation de la SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error while initializing SDL: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Version d'OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Double Buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    glEnable(GL_DEPTH_TEST);

    // taille de la fenêtre par défaut
    window_width = 1024;
    window_height = 680;

    // Création de la fenêtre
    window = SDL_CreateWindow("Dungeon Master", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if(window == 0)
    {
        std::cout << "Error while creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Création du contexte OpenGL
    context_gl = SDL_GL_CreateContext(window);
    if(context_gl == 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // GLEW
    glewExperimental = GL_TRUE;
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        exit(EXIT_FAILURE);
    }
}

View::~View() {
    SDL_GL_DeleteContext(context_gl);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void View::update() {
    player_input = INPUT_NONE;
    if(SDL_PollEvent(&event))
    {
        if(event.window.event == SDL_WINDOWEVENT_CLOSE)
            player_input = INPUT_QUIT;
    }
}

void View::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    Mesh mesh;
    mesh.loadFromFile("/home/datross/Programmation/C-C++/dungeonMaster/res/fauteuil.obj");
    mesh.loadShader("/home/datross/Programmation/C-C++/dungeonMaster/res/shaders/3D.vs.glsl",
                    "/home/datross/Programmation/C-C++/dungeonMaster/res/shaders/directionallight.fs.glsl");
    Camera camera;
    camera.init(70, 1.);
    camera.position = glm::vec3(-4,8,10);
    camera.direction = glm::vec3(0.5,-0.5,-1);
    
    glm::mat4 v = camera.getVMatrix();
    glm::mat4 mv = v;
    //mv = glm::scale(v, glm::vec3(0.1,0.1,0.1));
    mv = glm::translate(mv, glm::vec3(0,0,0));
//     glm::mat4 mv = glm::translate(glm::mat4(1.), glm::vec3(0,0,1));
    
    mesh.activateShader();
    
    mesh.setMVMatrix(mv);
//     std::cout << camera.getPMatrix() * mv << std::endl;
//     mesh.setMVPMatrix(mv);
    mesh.setMVPMatrix(camera.getPMatrix() * mv);
    //mesh.setNormalMatrix(glm::transpose(glm::inverse(mv)));
    mesh.setNormalMatrix(glm::mat4(1.));
    //std::cout << glm::mat4(1.) << std::endl;
    mesh.setShininess(1.);
    mesh.setLightDir_vs(glm::vec3(1,0,0));
    mesh.setLightIntensity(glm::vec3(1,1,1));
    mesh.setKs(1.);
    mesh.setKd(1.);
    mesh.render();

    SDL_GL_SwapWindow(window);
}

Player_input View::get_input() {
    return player_input;
}
