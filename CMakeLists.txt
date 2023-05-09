cmake_minimum_required(VERSION 3.23)
project(SDLGameTest)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)
include_directories(SDL2Test ${SDL2_INCLUDE_DIRS} ${_sdl2image_incdir})

add_executable(SDLGameTest main.cpp BoardGame.cpp BoardGame.h BoardGameAI.cpp BoardGameAI.h)
target_link_libraries(SDLGameTest ${SDL2_LIBRARIES} SDL2_image::SDL2_image)
configure_file(chessboard.png chessboard.png COPYONLY)
configure_file(whitepawn.png whitepawn.png COPYONLY)
configure_file(blackpawn.png blackpawn.png COPYONLY)
configure_file(border.png border.png COPYONLY)
