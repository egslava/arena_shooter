#pragma once

#include <stdio.h>
#include <chrono>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Shader;
class VBO;
class VAO;
class FPSCounter;
class MyException;
class MyGlException;
class MyIllegalStateException;
class MySDLException;
class MyShaderException;
class Program;

#include "fpscounter.h"
#include "exceptions.h"
#include "gapi/shader.h"
#include "gapi/vbo.h"
#include "gapi/program.h"
#include "gapi/vao.h"
#include "gapi/texture.h"
