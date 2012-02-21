cmake_minimum_required(VERSION 2.6)
project(application CXX C)

option(DEBUG "Build for debugging." ON) 

#add_subdirectory(...)

if(NOT $ENV{CPLUS_INCLUDE_PATH} STREQUAL "")
  string(REPLACE ":" " " INCLUDES $ENV{CPLUS_INCLUDE_PATH})
  string(REPLACE "//" "/" INCLUDES ${INCLUDES})
  string(REGEX MATCHALL "[^ ]+" LIST_INCLUDES ${INCLUDES})
endif(NOT $ENV{CPLUS_INCLUDE_PATH} STREQUAL "")

if(NOT $ENV{LDPATH} STREQUAL "")
  string(REPLACE ":" " " LDPATH $ENV{LDPATH})
  string(REPLACE "//" "/" LDPATH ${LDPATH})
  string(REGEX MATCHALL "[^ ]+" LIST_LDPATH ${LDPATH})
endif(NOT $ENV{LDPATH} STREQUAL "")

find_library(3DS 3ds PATHS ${LIST_LDPATH})
find_library(OPENAL openal ${LIST_LDPATH})
find_library(OPENGL GL ${LIST_LDPATH})
find_library(GLEW GLEW ${LIST_LDPATH})
find_library(GLU GLU ${LIST_LDPATH})
find_library(SDL SDL ${LIST_LDPATH})
find_library(SDL_IMAGE SDL_image ${LIST_LDPATH})
find_library(SDL_NET SDL_net ${LIST_LDPATH})
find_library(SDL_TTF SDL_ttf ${LIST_LDPATH})
find_library(TINYXML tinyxml ${LIST_LDPATH})

add_definitions(-std=c++0x -fPIC -Wall)
add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_LINUX -DZENI_DLL= -DZENI_EXT= -DZENI_AUDIO_DLL= -DZENI_AUDIO_EXT= -DZENI_CORE_DLL= -DZENI_CORE_EXT= -DZENI_GRAPHICS_DLL= -DZENI_GRAPHICS_EXT= -DZENI_NET_DLL= -DZENI_NET_EXT= -DZENI_REST_DLL= -DZENI_REST_EXT= -DDISABLE_DX9 -DDISABLE_WGL -DDISABLE_CG -DX64 -ffast-math)

if(DEBUG)
  add_definitions(-D_DEBUG -DTEST_NASTY_CONDITIONS -ggdb)
else(DEBUG)
  add_definitions(-O2 -fexpensive-optimizations -funroll-loops)
endif(DEBUG)

include_directories(BEFORE include ${LIST_INCLUDES})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
set(LIBRARY_OUTPUT_DIRECTORY lib)
set(ARCHIVE_OUTPUT_DIRECTORY lib)

# zeni
add_library(zeni SHARED src/zeni.cxx)
target_link_libraries(zeni zeni ${TINYXML})
# zeni_audio
add_library(zeni_audio SHARED src/zeni_audio.cxx)
target_link_libraries(zeni_audio zeni ${OPENAL})
# zeni_core
add_library(zeni_core SHARED src/zeni_core.cxx)
target_link_libraries(zeni_core zeni ${SDL})
# zeni_graphics
add_library(zeni_graphics SHARED src/zeni_graphics.cxx)
target_link_libraries(zeni_graphics zeni ${SDL_TTF} ${SDL_IMAGE} ${SDL} ${GLEW} ${GLU} ${OPENGL} ${3DS})
# zeni_net
add_library(zeni_net SHARED src/zeni_net.cxx)
target_link_libraries(zeni_net zeni ${SDL_NET} ${SDL})
# zeni_rest
add_library(zeni_rest SHARED src/zeni_rest.cxx)
target_link_libraries(zeni_rest zeni_graphics zeni_audio zeni)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY .)
set(RUNTIME_OUTPUT_DIRECTORY .)

add_executable(application src/application.cxx)
target_link_libraries(application zeni_rest zeni_rest zeni_net zeni_graphics zeni_core zeni_audio zeni)

#install(...)
#
#include(InstallRequiredSystemLibraries)
#set(CPACK_RESOURCE_FILE_LICENSE "docs/README.txt")
#set(CPACK_PACKAGE_VERSION_MAJOR "4")
#set(CPACK_PACKAGE_VERSION_MINOR "2")
#include(CPack)
