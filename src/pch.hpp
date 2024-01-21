#pragma once

#include <cassert>
#include <chrono>
#include <filesystem>
#include <functional>
#include <iostream>
#include <istream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

#include "fmt/core.h"

#include "glad/glad.h"

#include "SDL.h"
#include "SDL_events.h"
#include "SDL_opengl.h"
#include "SDL_timer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/intersect.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/transform.hpp"
