#include <stdio.h>
#include "Config.h"

#define GL_GLEXT_PROTOTYPES
#define GL3_PROTOTYPES


#include "MathFunctions/mysqrt.h"


#ifdef USE_MYMATH
#include "mysqrt.h"
//#include "Table.h"
#endif

//#include <glvnd/>

#include <string>
#include <memory>
#include <exception>
#include <vector>

#include "gapi/gapi.h"
#include "scene/camera.h"
#include "scene/scene.h"
//#include <GL/glu.h>

#include <math.h>
#include "math/math.h"

#include "game/bullet.h"



#include "game/level.h"


#include <iostream>
#include <array>

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WIN32
	#define WINDOWS  /* uncomment this line to use it for windows.*/
#endif
#ifdef WINDOWS
	#include <direct.h>
	#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include<iostream>

std::string GetCurrentWorkingDir( void ) {
  char buff[FILENAME_MAX];
  GetCurrentDir( buff, FILENAME_MAX );
  std::string current_working_dir(buff);
  return current_working_dir;
}

//#define SDL_MAIN_HANDLED

int main (int argc, char *argv[]){

#ifndef NDEBUG
    printf("It's a debug build\n");
#endif

#ifdef NDEBUG
    printf("It's a release build\n");
#endif
    std::cout << GetCurrentWorkingDir() << std::endl;
	printf("Hello world! Version is: %i.%i \n", Arena_VERSION_MAJOR, Arena_VERSION_MINOR);

#ifdef HAVE_LOG
    printf("The LOG function has been found.\n");
#endif

    try {
        MyAppCallback callback;
        MySDLApp app(callback);
        app.loop();
    } catch (MyException &e) {
        std::cout << e.what() << std::endl;
        throw;
        return 1;
    }

	return 0;
}
