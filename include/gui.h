#ifndef GUI_H
#define GUI_H

#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

class Gui {
public:
	static Gui& getInstance()
        {
            static Gui instance; // Guaranteed to be destroyed.
								 // Instantiated on first use.
            return instance;
        }

 	   Gui(Gui const&)             = delete;
 	   void operator=(Gui const&)  = delete;

	   // Boolean Main menu
	   bool  showMainMenu,
	   		 showLevelSelector,
	   		 showSavesSelector,
	   		 showOptionsSelector,
	   		 showQuitPopUp;

private:
	Gui() : showMainMenu(true),
		  showLevelSelector(false),
		  showSavesSelector(false),
		  showOptionsSelector(false),
		  showQuitPopUp(false) {}                    // Constructor? (the {} brackets) are needed here.
};

#endif
