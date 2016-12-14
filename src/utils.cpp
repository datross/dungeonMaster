#include "utils.h"
#include <cmath>

float Utils::distance(glm::ivec2 a, glm::ivec2 b) {
	return sqrt(pow(b.x-a.x,2)+pow(b.y-a.y,2));
}
