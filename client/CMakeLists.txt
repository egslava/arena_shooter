cmake_minimum_required (VERSION 2.8)
project (Arena)

set(Arena_VERSION_MAJOR 0)
set(Arena_VERSION_MINOR 0)

set(Arena_RES_DIR "${CMAKE_SOURCE_DIR}/res/")

macro (set_x86_64 out_var case86 case64)
	# thx to: https://stackoverflow.com/a/39258787	
	if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")  
		set( ${out_var} ${case86})
	else()
		set( ${out_var} ${case64})
	endif()
endmacro()

macro (set_debug_release out_var debug_case release_case)
	if(CMAKE_BUILD_TYPE MATCHES DEBUG)
		set(${out_var} ${debug_case})
	else()
		set(${out_var} ${release_case})
	endif()
endmacro()

macro(copy_files_if_different target_suffix to_dir from_dir)
	# The Visual Studio target WILL ALWAYS BE OUTDATED. So it will always
	# attempt to copy the files again and again However, the copy method
	# itself will copy files only if they are different.

	# 1. Create a new directory
	# https://stackoverflow.com/questions/3702115/creating-a-directory-in-cmake
	add_custom_target(
		"copy-${target_suffix}" ALL
		COMMAND ${CMAKE_COMMAND} -E make_directory "${to_dir}"
	)

	# 2. Enumerating all the files
	file(GLOB DL_FILES ${from_dir})

	# 3. Create a command to copy file from one place to ${to_dir}
	foreach(DL_FILE ${DL_FILES})
		add_custom_command(
			TARGET "copy-${target_suffix}"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different  
				"${DL_FILE}"
				${to_dir}
		)
	endforeach()	
endmacro()

if(WIN32)

	# <SDL2>
	# For FindSDL2.cmake and others:
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
	set(SDL2_DIR ${SDL2_DIR}  "${CMAKE_SOURCE_DIR}/libs/SDL2/win_x86_64/SDL2-2.0.9")
	# </SDL2>


	# <GLEW>
	set(GLEW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/GLEW/win32/glew-2.1.0/include")
	# thx to: https://cmake.org/pipermail/cmake/2012-March/049620.html
	set_debug_release (GLEW_DEBUG_RELEASE_LIB "s" "")
	set_x86_64(GLEW_32_64_PLATFORM "Win32" "x64")

	set(GLEW_LIBRARY "${CMAKE_SOURCE_DIR}/libs/GLEW/win32/glew-2.1.0/lib/Release/${GLEW_32_64_PLATFORM}/glew32${GLEW_DEBUG_RELEASE_LIB}.lib")


	set_x86_64(DL_DIR "${CMAKE_SOURCE_DIR}/libs/dynlibs/win_x86" "${CMAKE_SOURCE_DIR}/libs/dynlibs/win_x64")


	copy_files_if_different("dyn_libs" "$<TARGET_FILE_DIR:Arena>" "${DL_DIR}/*")
	copy_files_if_different("res-build" "$<TARGET_FILE_DIR:Arena>/res" "${CMAKE_SOURCE_DIR}/res/*")
	copy_files_if_different("res-debug" "${CMAKE_BINARY_DIR}/res" "${CMAKE_SOURCE_DIR}/res/*")

	# message("Path from SDL2.dll: ${SDL2_DYNLIB_FROM}")
	# message("Path from GLEW: ${GLEW_DYNLIB_FROM}")
	# message("Path to SDL2.dll: ${SDL2_DYNLIB_TO}")
	# message("Path to GLEW: ${GLEW_DYNLIB_TO}")
    
	# </GLEW>
endif(WIN32)

if(UNIX)
    copy_files_if_different("res" "${CMAKE_BINARY_DIR}/res" "${CMAKE_SOURCE_DIR}/res/*")
endif(UNIX)


# SDL 2
#message("Hello!")
#find_file(SDL2_INCLUDE_DIR NAME SDL.h HINTS SDL2)
#find_library(SDL2_LIBRARY NAME SDL2)
set(SDL_BUILDING_LIBRARY ON)
#include (FindSDL)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(EXTRA_LIBS ${EXTRA_LIBS} ${SDL2_LIBRARIES})

message(${SDL2_INCLUDE_DIRS})
message(${SDL2_LIBRARIES})

# https://cmake.org/cmake/help/v3.0/module/FindOpenGL.html
#set (OpenGL_GL_PREFERENCE GLVND)
set (OpenGL_GL_PREFERENCE LEGACY)  # probably, should use GLVND instead,
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
set(EXTRA_LIBS ${EXTRA_LIBS} ${OPENGL_gl_LIBRARY})

find_package (GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

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
    "arena.cpp"
    "src/*.cpp"
    "src/*/*.cpp"
    "src/*/*/*.cpp"
)
# </thx to: https://stackoverflow.com/a/51008650/1444191>

            # <--this is out-file path
include_directories("./h")
add_executable(Arena ${source_files} )
target_link_libraries(Arena ${EXTRA_LIBS})

# thx to: https://stackoverflow.com/a/10672739 
# and https://cmake.org/cmake/help/v3.0/command/add_custom_command.html 
# and https://stackoverflow.com/questions/40696990/cmake-post-build-event-copy-compiled-libraries
# add_custom_command(
# 	TARGET Arena 
# 	PRE_BUILD        # Adds a post-build event to MyTest
#     COMMAND ${CMAKE_COMMAND} -E copy_directory  # which executes "cmake - E copy_if_different..."
# 		${Arena_RES_DIR}
# 		${PROJECT_BINARY_DIR}
# )
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
