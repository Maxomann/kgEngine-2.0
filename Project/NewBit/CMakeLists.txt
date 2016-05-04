cmake_minimum_required(VERSION 2.8.12)
project(NewBit)

#NewBit
include_directories("include/NewBit")
file(GLOB_RECURSE NEWBIT_HEADERS "include/NewBit/*.h")
file(GLOB_RECURSE NEWBIT_SOURCES "src/NewBit/*.cpp")

#TileMapGame
include_directories("include/TileMapGame")
file(GLOB_RECURSE TILEMAPGAME_HEADERS "include/TileMapGame/*.h")
file(GLOB_RECURSE TILEMAPGAME_SOURCES "src/TileMapGame/*.cpp")

link_directories( "./Debug" )
link_directories( "./Release" )

#Link
set ( PROJECT_LINK_LIBS sfml-audio.lib sfml-graphics.lib sfml-main.lib sfml-network.lib sfml-system.lib sfml-window.lib tgui.lib thor.lib glew32.lib OpenGL32.lib )
set ( PROJECT_LINK_LIBS_DEBUG sfml-audio-d.lib sfml-graphics-d.lib sfml-main-d.lib sfml-network-d.lib sfml-system-d.lib sfml-window-d.lib tgui-d.lib thor-d.lib glew32.lib OpenGL32.lib )

include_directories("../boost")
link_directories( "../boost/bin.v2/libs/thread/build/msvc-14.0/release/link-static/threading-multi" )
link_directories( "../boost/bin.v2/libs/system/build/msvc-14.0/release/link-static/threading-multi" )
link_directories( "../boost/bin.v2/libs/date_time/build/msvc-14.0/release/link-static/threading-multi" )
link_directories( "../boost/bin.v2/libs/chrono/build/msvc-14.0/release/link-static/threading-multi" )

link_directories( "../boost/bin.v2/libs/thread/build/msvc-14.0/debug/link-static/threading-multi" )
link_directories( "../boost/bin.v2/libs/system/build/msvc-14.0/debug/link-static/threading-multi" )
link_directories( "../boost/bin.v2/libs/date_time/build/msvc-14.0/debug/link-static/threading-multi" )
link_directories( "../boost/bin.v2/libs/chrono/build/msvc-14.0/debug/link-static/threading-multi" )

include_directories("../glew/include")
link_directories( "../glew/lib/Release/Win32" )

include_directories("../SFML/include")
link_directories( "../SFML/lib" )

include_directories("../Thor/include")
include_directories("../Thor/extlibs/aurora/include")
link_directories( "../Thor/src/Debug" )
link_directories( "../Thor/src/Release" )

include_directories("../TGUI/include")
link_directories( "../TGUI/lib/Release" )
link_directories( "../TGUI/lib/Debug" )

add_executable(NewBit ${NEWBIT_SOURCES} ${NEWBIT_HEADERS})
target_link_libraries (NewBit optimized ${PROJECT_LINK_LIBS})
target_link_libraries (NewBit debug ${PROJECT_LINK_LIBS_DEBUG})

SET_TARGET_PROPERTIES( NewBit
	PROPERTIES
	DEBUG_OUTPUT_NAME NewBit-d
	RELEASE_OUTPUT_NAME NewBit
)
set_target_properties( NewBit
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}../../../Build"
)


add_library(TileMapGame SHARED ${TILEMAPGAME_SOURCES} ${TILEMAPGAME_HEADERS})
target_link_libraries (TileMapGame debug ${PROJECT_LINK_LIBS_DEBUG} NewBit-d.lib)
target_link_libraries (TileMapGame optimized ${PROJECT_LINK_LIBS} NewBit.lib)

SET_TARGET_PROPERTIES( TileMapGame
	PROPERTIES
	DEBUG_OUTPUT_NAME TileMapGame-d
	RELEASE_OUTPUT_NAME TileMapGame
)
set_target_properties( TileMapGame
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}../../../Build"
)