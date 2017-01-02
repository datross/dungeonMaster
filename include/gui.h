#ifndef GUI_H
#define GUI_H

#include <vector>
#include <utility>

#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "FilePath.h"

class Gui {
public:
	static Gui& getInstance(){
        static Gui instance; // Guaranteed to be destroyed.
							 // Instantiated on first use.
        return instance;
    }

	void errorWindow(bool& isValidate);
	void loadTexture (std::vector< std::pair<ImTextureID, ImVec2> >& container, glimac::FilePath path);

	Gui(Gui const&)             = delete;
	void operator=(Gui const&)  = delete;

	// Boolean Main menu
	bool  showMainMenu,
		  showLevelSelector,
		  showSavesSelector,
		  showOptionsSelector,
		  showQuitPopUp;

	// Boolean HUD
	bool showHUDIndicators,
		 showHUDInventory,
		 showHUDMap,
		 showHUDOptions,
		 showHUDEndLevelPopUp;

	// Textures menu (id and size)
	std::vector< std::pair<ImTextureID, ImVec2> > mainMenuTex;
	std::vector< std::pair<ImTextureID, ImVec2> > HUDTex;


private:
	Gui();
};

#endif
