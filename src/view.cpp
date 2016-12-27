#include <iostream>
#include "view.h"


#include "gui.h"

View::View()
    : player_input(INPUT_NONE) {
    // Initialisation de la SDL
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        std::cout << "Error while initializing SDL: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
	// Setup window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);

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

    // Setup ImGui binding
    ImGui_ImplSdlGL3_Init(window);
}

View::~View() {
    ImGui_ImplSdlGL3_Shutdown();
    SDL_GL_DeleteContext(context_gl);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void View::update() {
    updateEvent();
}

void View::updateEvent() {
    player_input = INPUT_NONE;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT) {
            player_input = INPUT_QUIT;
        } else if(event.type == SDL_KEYDOWN) {
            if(event.key.keysym.sym == SDLK_z) {
                player_input = INPUT_MOVE_FORWARD;
            } else if(event.key.keysym.sym == SDLK_q) {
                player_input = INPUT_MOVE_LEFT;
            } else if(event.key.keysym.sym == SDLK_s) {
                player_input = INPUT_MOVE_BACKWARD;
            } else if(event.key.keysym.sym == SDLK_d) {
                player_input = INPUT_MOVE_RIGHT;
            }
        }
    }
}

void View::reshape(unsigned w, unsigned h) {
    window_width  = w;
    window_height = h;
    float aspect_ratio = (float)w/h;

    // update des matrices de projection des players
    // TODO gérer proprement la fov des camera
    for(auto it = assets_ptr->map.players.begin(); it != assets_ptr->map.players.end(); ++it) {
        it->cam.init(70., aspect_ratio);
    }
}

void View::render(Game_state game_state) {
    glClear(GL_COLOR_BUFFER_BIT);

//     Mesh mesh;
//     mesh.loadFromFile("res/tete.obj");
//     mesh.loadShader("res/shaders/3D.vs.glsl",
//                     "res/shaders/directionallight.fs.glsl");
//     Camera camera;
//     camera.init(70, 1.);
//     camera.position = glm::vec3(map_ptr->players.begin()->position.x, 0.5, map_ptr->players.begin()->position.y);
//     camera.direction = glm::vec3(0.5,-0.5,-1);
//
//     glm::mat4 v = camera.getVMatrix();
//     glm::mat4 mv = v;
//     //mv = glm::scale(v, glm::vec3(0.1,0.1,0.1));
//     mv = glm::translate(mv, glm::vec3(0,0,0));
// //     glm::mat4 mv = glm::translate(glm::mat4(1.), glm::vec3(0,0,1));
//
//     mesh.activateShader();
//
//     mesh.setMVMatrix(mv);
//     mesh.setMVPMatrix(camera.getPMatrix() * mv);
//     mesh.setNormalMatrix(glm::transpose(glm::inverse(mv)));
//     mesh.setNormalMatrix(glm::mat4(1.));
//     mesh.setShininess(1.);
//     mesh.setLightDir_vs(glm::vec3(1,1,1));
//     mesh.setLightIntensity(glm::vec3(1,1,1));
//     mesh.setKs(glm::vec3(1,1,1));
//     mesh.setKd(glm::vec3(1,1,1));
//     mesh.render();

	// USER INTERFACE
	ImGui_ImplSdlGL3_NewFrame(window);
	if(game_state == STATE_MENU) {
		mainMenu();
	} else if (game_state == STATE_GAMEPLAY) {

	} else if (game_state == STATE_QUIT) {

	} else {

	}
	ImGui::Render();

    SDL_GL_SwapWindow(window);
}

Player_input View::get_input() {
    return player_input;
}

void View::mainMenu(){

	//Menu's textures loading
	float tex_w = (float)ImGui::GetIO().Fonts->TexWidth;
	float tex_h = (float)ImGui::GetIO().Fonts->TexHeight;
	ImTextureID tex_id = ImGui::GetIO().Fonts->TexID;

	// Booleans for menu's windows
	bool showMainMenu = true,
		 showLevelSelector = false,
		 showSavesSelector = false,
		 showOptionsSelector = false,
		 showQuitPopUp = false;

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	// background
		//ImGui::Image(tex_id, ImVec2(window_width, window_height), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));

	//MAIN MENU
	if(ImGui::Begin("Main menu", &showMainMenu, window_flags)){

		//Buttons
		if (ImGui::Button("Nouvelle partie")){
			showMainMenu = false;
			showLevelSelector = true;
		}
		if (ImGui::Button("Charger partie")){
			showMainMenu = false;
			showSavesSelector = true;
		}
		if (ImGui::Button("Options")){
			showMainMenu = false;
			showOptionsSelector = true;
		}
		if (ImGui::Button("Quitter")){
			ImGui::OpenPopup("Quit");
			//QUIT CONFIRMAION POPUP
		}
		if (ImGui::BeginPopup("Quit")){

			ImGui::Spacing();
			ImGui::Text("Voulez vous vraiment quitter le jeu ?");
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::Button("Oui")){
				player_input = INPUT_QUIT;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("non"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		/*if (ImGui::ImageButton(tex_id, ImVec2(32,32), ImVec2(0,0), ImVec2(32.0f/tex_w,32/tex_h), 1, ImColor(0,0,0,255))) {

		}*/

		ImGui::End();
	}
}

void View::setAssets(Assets& _assets_ptr) {
    assets_ptr = std::shared_ptr<Assets>(&_assets_ptr);
}
