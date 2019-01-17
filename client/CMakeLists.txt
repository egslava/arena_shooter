cmake_minimum_required (VERSION 2.8)
project (Arena)

set(Arena_VERSION_MAJOR 0)
set(Arena_VERSION_MINOR 0)


## glfw3
#find_package(glfw3 REQUIRED)
#set (EXTRA_LIBS ${EXTRA_LIBS} glfw)

# SDL 2
#message("Hello!")
#find_file(SDL2_INCLUDE_DIR NAME SDL.h HINTS SDL2)
#find_library(SDL2_LIBRARY NAME SDL2)
set(SDL_BUILDING_LIBRARY ON)
#include (FindSDL)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(EXTRA_LIBS ${EXTRA_LIBS} ${SDL2_LIBRARIES})

# https://cmake.org/cmake/help/v3.0/module/FindOpenGL.html
#set (OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
message("${OPENGL_INCLUDE_DIR}")
message("${OPENGL_gl_LIBRARY}")
include_directories(${OPENGL_INCLUDE_DIR})
set(EXTRA_LIBS ${EXTRA_LIBS} ${OPENGL_gl_LIBRARY})

include (CheckFunctionExists)
check_function_exists(printf HAVE_LOG)

configure_file(
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
)

include_directories( "${PROJECT_BINARY_DIR}" )

#if (USE_MYMATH)
#    include_directories("${PROJECT_SOURCE_DIR}/MathFunctions")
#    add_subdirectory(MathFunctions)
#    set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
#endif (USE_MYMATH)
## enable_testing()


# <thx to: https://stackoverflow.com/a/51008650/1444191>
file(GLOB source_files
#        "*.h"
#        "Classes/*/*.cpp"
#        "Classes/*/*.h"
        "*.cpp"
        "*/*.cpp"
)
# </thx to: https://stackoverflow.com/a/51008650/1444191>

add_executable(Arena arena.cpp ${source_files} )
target_link_libraries(Arena ${EXTRA_LIBS})
# add_test(ArenaTest  echo "Ha" | grep "Hah" )

option (USE_MYMATH "Use tutorial provided math implementation" ON)

install (TARGETS Arena DESTINATION bin)
#install (FILES "${PROJECT_BINARY_DIR}/Config.h" DESTINATION include)

include (CTest)
add_test (ArenaRuns Arena 25)
#add_test (ArenaUsage Arena)
#set_tests_properties(ArenaUsage PROPERTIES PASS_REGULAR_EXPRESSION "Usage: .*number")

include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE
    "${CMAKE_CURRENT_SOURCE_DIR}/dist/License.txt" )
set (CPACK_PACKAGE_VERSION_MAJOR "${Arena_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${Arena_VERSION_MINOR}")
include (CPack)
