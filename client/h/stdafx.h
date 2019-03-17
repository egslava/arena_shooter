#ifndef STDAFX_H
#define STDAFX_H

#include <stdio.h>
#include <chrono>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <GL/gl.h>

#ifdef WIN32
#include <Windows.h>
#define uint GLuint
#define uint32 GLuint
#define uint64 GLuint64
#define uint8	
#endif 
//#include <GL/glext.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#endif // STDAFX_H
