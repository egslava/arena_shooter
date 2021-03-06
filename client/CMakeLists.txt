cmake_minimum_required (VERSION 2.8)
project (Arena)

# thx: https://stackoverflow.com/a/44023223
set_property(GLOBAL PROPERTY USE_FOLDERS ON) 
# /thx

set(Arena_VERSION_MAJOR 0)
set(Arena_VERSION_MINOR 0)

set(Arena_RES_DIR "${CMAKE_SOURCE_DIR}/res/")
set(Arena_CONVERT_OBJ_COMMAND "${CMAKE_SOURCE_DIR}/../utils/models.py")

add_executable(Arena)

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

function(change_ext out_path in_path new_ext)
    # change_ext(result ${CMAKE_SOURCE_DIR}/file.png ".PNG") -> result = ${CMAKE_SOURCE_DIR}/file.PNG
    get_filename_component(_name_we ${in_path} NAME_WE)
    get_filename_component(_path ${in_path} DIRECTORY)
    set(${out_path} "${_path}/${_name_we}${new_ext}" PARENT_SCOPE)
endfunction()

macro (add_copy_file_command target source dest)
    get_filename_component(_to_dir ${dest} DIRECTORY)         # _from_root = "~/my/arena-shooter/client/res"
    add_custom_command(TARGET ${target} COMMAND ${CMAKE_COMMAND} -E make_directory "${_to_dir}")
    add_custom_command(TARGET ${target} COMMAND ${CMAKE_COMMAND} -E copy_if_different ${source} ${dest} VERBATIM )
endmacro()

function (add_custom_sources)
    set(options VERBATIM)
    set(oneValueArgs TARGET OUTPUT)
    set(multiValueArgs DEPENDS COMMAND)

    # Problem
    # -------
    # CMake doesn't support generator expressions for OUTPUT directories.
    # Thus, it's not gonna work:
    # add_custom_command(
    #    OUTPUT "$<TARGET_FILE_DIR:Main>/gen/file1"
    #    DEPENDS file1
    #    COMMAND blah
    # )
    # target_sources(Main "$<TARGET_FILE_DIR:Arena>/gen/file1")
    #
    # So there will be problems with Debug/Release configurations
    # in MS VS and Qt and Qt Creator and need to work it around.
    #
    # The section below contains the code with exactly the same logic.
    #
    # Using
    # -----
    # add_custom_sources(
    #    TARGET Main
    #    OUTPUT "$<TARGET_FILE_DIR:Main>/gen/file1"
    #    DEPENDS file1
    #    COMMAND blah
    # )
    # TARGET - is the target where your generated source code/image/3d-model/anything should be added as a dependency.
    #
    # How it works
    # ------------
    # 1. Replacing generator expressions to something unique.
    #    I map such name "$<TARGET_FILE_DIR:Arena>/gen/file1"
    #    to somewhat like: "genexpr/TARGET_FILE_DIR_Arena/gen/file1.shadow"
    # 2. make all the subdirectories to SHADOW and the original file
    # 3. add_custom_command on shadow file. I add TOUCH command to
    #    create/update shadow file as well
    # 4. target_sources(SHADOW_FILE)

    # 1. Parsing args
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    if ( ${ARG_VERBATIM} )
        set(ARG_VERBATIM VERBATIM)
    else()
        unset(ARG_VERBATIM)
    endif()

    # 2. Getting SHADOW name
    string(REGEX REPLACE "\\$<(.+)>" "\\1" _no_genex_file ${ARG_OUTPUT} )
    string(REPLACE ":" "_" _no_genex_file ${_no_genex_file} )
    set(_no_genex_file "${_no_genex_file}.shadow")
    set(_no_genex_file "${CMAKE_BINARY_DIR}/genex/${_no_genex_file}")

    # 3. Making all the dirs
    get_filename_component(_no_genex_dir ${_no_genex_file} DIRECTORY)         # _from_root = "~/my/arena-shooter/client/res"
    get_filename_component(_output_dir ${ARG_OUTPUT} DIRECTORY)

    # 4. Running the command and TOUCHing SHADOW-file
    add_custom_command(
        OUTPUT ${_no_genex_file}
        DEPENDS ${ARG_DEPENDS}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_output_dir}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_no_genex_dir}
        COMMAND ${CMAKE_COMMAND} -E touch ${_no_genex_file}
        COMMAND ${ARG_COMMAND}
        ${ARG_VERBATIM}
    )

    # 5. Adding the dependency, so both files (the original and the shadow)
    #    will be updated when the source file is changed
	source_group(".shadows" FILES ${_no_genex_file})
    target_sources(Arena PUBLIC ${_no_genex_file})
endfunction()


macro(copy_files_if_different target_suffix to_dir from_regexp)
    # The Visual Studio target WILL ALWAYS BE OUTDATED. So it will always
    # attempt to copy the files again and again However, the copy method
    # itself will copy files only if they are different.

    # set(_target "copy-${target_suffix}")                                # _target    = "copy_res"
                                                                        # from_regexp= "~/my/arena-shooter/client/res/*"
                                                                        # to_dir     = "~/my/arena-shooter/client/build/res"
    get_filename_component(_from_root ${from_regexp} DIRECTORY)         # _from_root = "~/my/arena-shooter/client/res"

    # 1. Create a new directory
    # https://stackoverflow.com/questions/3702115/creating-a-directory-in-cmake
#    add_custom_target(
#        ${_target}
#        COMMENT "Recursively copying directory ${from_regexp} to ${to_dir}"
#    )

#    add_dependencies(Arena ${_target})
    add_custom_command(
        TARGET Arena
        COMMAND ${CMAKE_COMMAND} -E make_directory "${to_dir}"
        VERBATIM
    )

    # 2. Enumerating all the files
    file(GLOB_RECURSE _files ${from_regexp})

    # 3. Create a command to copy file from one place to ${to_dir}
    foreach(_from_file ${_files})                                        # _from_file="~/my/arena-shooter/client/res/texture.png"
        # a. Getting a new filename
        string(REPLACE ${_from_root} ${to_dir} _to_file ${_from_file})   # _to_file="~/arena-shooter/build/res/hero/body.png

        # b. Creating a directory for the new file if it doesn't exist
        get_filename_component(_to_subdir ${_to_file} DIRECTORY)         # _to_subdir="~/arena-shooter/build/res/hero/"
        get_filename_component(__ext ${_to_file} EXT)         # __ext=".oBj"
#        message("from file: ${_from_file}\n"
#                "to file: ${_to_file}\n"
#                "to subdir: ${_to_subdir}\n"
#                "${__ext}\n"
#                "${__haha}\n")

        if (__ext)
            string(TOUPPER ${__ext} _ext)
        else()
            set(_ext "")
        endif()

        add_custom_command( TARGET Arena COMMAND ${CMAKE_COMMAND} -E make_directory ${_to_subdir} VERBATIM )

        if (_ext MATCHES "OBJ")
            change_ext(_to_file ${_to_file} ".model")

            add_custom_sources(
                TARGET Arena
                OUTPUT ${_to_file}
                DEPENDS ${Arena_CONVERT_OBJ_COMMAND} ${_from_file}
                COMMAND python ${Arena_CONVERT_OBJ_COMMAND} ${_from_file} ${_to_file}
                VERBATIM
            )
#            target_sources(Arena PUBLIC ${_to_file})

        elseif(_ext MATCHES "PNG" OR _ext MATCHES "JPG")
            change_ext(_to_file ${_to_file} ".pvr")

            add_custom_sources(
                TARGET Arena
                OUTPUT ${_to_file}
                DEPENDS ${_from_file}
                COMMAND PVRTexToolCLI -i ${_from_file} -o ${_to_file} -flip y -f R8G8B8A8
                VERBATIM
            )
#            target_sources(Arena PUBLIC ${_to_file})
        else()
            # c. Copying the file to the new location
            add_custom_command(
                TARGET Arena
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_from_file} ${_to_file} VERBATIM )
#            add_custom_commant(
#                TARGET Arena
#                OUTPUT ${_to_file}
#                DEPENDS ${_from_file}
#                COMMAND "${CMAKE_COMMAND} -E copy_if_different ${_from_file} ${_to_file}"
#                VERBATIM
#            )
        endif()
#        message("from file: ${_from_file}\nto file: ${_to_file}\n")
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

   	# Thx to: https://stackoverflow.com/a/37994396. 
   	# It sets up Arena project as a default one in MS VS, instead of ALL_BUILD
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Arena)
    # /thxto
endif(WIN32)

if(UNIX)

#    message("_out: ${_out}")
    #    string(REPLACE _out _replaced _out "$<TARGET_FILE_DIR:Arena>/res" )

    copy_files_if_different("res" "$<TARGET_FILE_DIR:Arena>/res" "${CMAKE_SOURCE_DIR}/res/*")
#    copy_files_if_different("res" "${CMAKE_BINARY_DIR}/res" "${CMAKE_SOURCE_DIR}/res/*")
endif(UNIX)


# SDL 2
#message("Hello!")
#find_file(SDL2_INCLUDE_DIR NAME SDL.h HINTS SDL2)
#find_library(SDL2_LIBRARY NAME SDL2)
set(SDL_BUILDING_LIBRARY ON)
#include (FindSDL)
find_package(SDL2 REQUIRED)

STRING(LENGTH ${SDL2_INCLUDE_DIRS} SDL2_PATH_LEN)
MATH(EXPR SDL2_PATH_LEN ${SDL2_PATH_LEN}-5)  # removing "/SDL2" part
STRING(SUBSTRING ${SDL2_INCLUDE_DIRS} 0 ${SDL2_PATH_LEN} SDL2_INCLUDE_DIRS)
message("SDL2 include dirs: " ${SDL2_INCLUDE_DIRS})

target_include_directories(Arena PUBLIC ${SDL2_INCLUDE_DIRS})
set(EXTRA_LIBS ${EXTRA_LIBS} ${SDL2_LIBRARIES})
# set(EXTRA_LIBS ${EXTRA_LIBS} "GLU")

# https://cmake.org/cmake/help/v3.0/module/FindOpenGL.html
#set (OpenGL_GL_PREFERENCE GLVND)
set (OpenGL_GL_PREFERENCE LEGACY)  # probably, should use GLVND instead,
find_package(OpenGL REQUIRED)
target_include_directories(Arena PUBLIC ${OPENGL_INCLUDE_DIR})
set(EXTRA_LIBS ${EXTRA_LIBS} ${OPENGL_gl_LIBRARY})

find_package (GLEW REQUIRED)
target_include_directories(Arena PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(Arena ${GLEW_LIBRARIES})

#include (CheckFunctionExists)
#check_function_exists(printf HAVE_LOG)

configure_file(
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
)

target_include_directories(Arena PUBLIC  "${PROJECT_BINARY_DIR}" )


# <thx to: https://stackoverflow.com/a/51008650/1444191> and to Alex Ushanov (@megaxela) from pro.buildtools
file(GLOB_RECURSE source_files "arena.cpp" "src/*.cpp" )
source_group("Source Files\\main" FILES "${source_files}")
target_sources(Arena PUBLIC ${source_files} )
# </thx to: https://stackoverflow.com/a/51008650/1444191>

            # <--this is out-file path
message("EXTRA_LIBS: ${EXTRA_LIBS}")
target_include_directories(Arena PUBLIC "./h")
target_link_libraries(Arena ${EXTRA_LIBS})

option (RUN_TESTS "Run all the test when the app starts app. Useful for debugging. " ON)

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


# This line [1]_ should be used to avoid many targets in Visual Studio:
# _[1]: https://github.com/opensoft/proofboot/blob/develop/cmake/ProofInternal.cmake#L206
