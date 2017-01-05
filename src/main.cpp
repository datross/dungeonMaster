#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>

#include "Program.h"
#include "FilePath.h"

#include "game.h"
#include "map.h"

using namespace glimac;

int main(int argc, char **argv)
{
   Game game(argv[0]);
   return game.run();
}
