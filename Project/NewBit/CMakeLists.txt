cmake_minimum_required(VERSION 2.8.12)
project(NewBit)
#set_property(GLOBAL PROPERTY DEBUG_CONFIGURATIONS Debug)

#NewBit
include_directories("include/NewBit")
file(GLOB_RECURSE NEWBIT_HEADERS "include/NewBit/*.h")
file(GLOB_RECURSE NEWBIT_SOURCES "src/NewBit/*.cpp")

#NewBit Source Groups
set(nb "include/NewBit/")

source_group("Header Files\\World" FILES ${nb}Component.h ${nb}ComponentManager.h ${nb}ComponentMissingException.h ${nb}System.h ${nb}SystemManager.h ${nb}World.h ${nb}Entity.h ${nb}ConfigFile.h ${nb}EntityFactory.h ${nb}EntityManager.h ${nb}SaveComponent.h)

source_group("Header Files\\Engine" FILES ${nb}Blueprint.h ${nb}BlueprintValueReadErrorException.h ${nb}Console.h ${nb}Engine.h ${nb}InputManager.h ${nb}PauseLock.h ${nb}PluginManager.h ${nb}PluginRegistrationException.h ${nb}PluginRequestException.h ${nb}RandomNumberGenerator.h ${nb}ResourceManager.h ${nb}Plugin.h ${nb}PluginFactory.h)

source_group("Header Files\\Core" FILES ${nb}Core.h ${nb}SaveManager.h ${nb}GameStateManager.h ${nb}GameState.h)

source_group("Header Files\\Tools" FILES ${nb}SwapContainer.h ${nb}Callback.h ${nb}WrongCallbackSignatureException.h ${nb}PathNotAvailableException.h ${nb}sfml_vector_extension.h)



#TileMapGame
include_directories("include/TileMapGame")
file(GLOB_RECURSE TILEMAPGAME_HEADERS "include/TileMapGame/*.h")
file(GLOB_RECURSE TILEMAPGAME_SOURCES "src/TileMapGame/*.cpp")

#TileMapGame SourceGroups
set(tm "include/TileMapGame/")

source_group("Header Files\\Components" FILES ${tm}AnimationComponent.h ${tm}CameraComponent.h ${tm}GraphicsComponent.h ${tm}TransformationComponent.h)

source_group("Header Files\\Systems\\Animation" FILES ${tm}AnimationSystem.h)

source_group("Header Files\\Systems\\Chunk" FILES ${tm}ChunkSystem.h ${tm}Chunk.h ${tm}ChunkContainer.h ${tm}ChunkGeneratorSystem.h ${tm}ChunkIOOperation.h ${tm}ChunkIOOperationQueue.h ${tm}WorldGenerator.h ${tm}WorldGeneratorManager.h)

source_group("Header Files\\Systems\\Graphics" FILES ${tm}GraphicsSystem.h ${tm}VBO.h ${tm}ChunkVBO.h ${tm}DrawingLayer.h ${tm}DrawingLayerContainer.h)

source_group("Header Files\\GameState" FILES ${tm}ConsoleGameState.h ${tm}DefaultGameState.h ${tm}SingleplayerGameState.h)



#Link
set ( PROJECT_LINK_LIBS optimized sfml-audio.lib optimized sfml-graphics.lib optimized sfml-main.lib optimized sfml-network.lib optimized sfml-system.lib optimized sfml-window.lib optimized tgui.lib optimized thor.lib )
set ( PROJECT_LINK_LIBS_DEBUG debug sfml-audio-d.lib debug sfml-graphics-d.lib debug sfml-main-d.lib debug sfml-network-d.lib debug sfml-system-d.lib debug sfml-window-d.lib debug tgui-d.lib debug thor-d.lib )
set ( PROJECT_LINK_LIBS_GENERAL general glew32.lib general OpenGL32.lib )

link_directories( "./Debug" )
link_directories( "./Release" )

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

#NewBit Executable
add_executable(NewBit ${NEWBIT_SOURCES} ${NEWBIT_HEADERS})
target_link_libraries (NewBit ${PROJECT_LINK_LIBS_GENERAL} ${PROJECT_LINK_LIBS_DEBUG} ${PROJECT_LINK_LIBS})

SET_TARGET_PROPERTIES( NewBit
	PROPERTIES
	DEBUG_OUTPUT_NAME NewBit-d
	RELEASE_OUTPUT_NAME NewBit
)
set_target_properties( NewBit
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}../../../Build"
)

#TileMapGame Library
add_library(TileMapGame SHARED ${TILEMAPGAME_SOURCES} ${TILEMAPGAME_HEADERS})
target_link_libraries (TileMapGame ${PROJECT_LINK_LIBS_GENERAL} ${PROJECT_LINK_LIBS_DEBUG} debug NewBit-d.lib ${PROJECT_LINK_LIBS} optimized NewBit.lib)

SET_TARGET_PROPERTIES( TileMapGame
	PROPERTIES
	DEBUG_OUTPUT_NAME TileMapGame-d
	RELEASE_OUTPUT_NAME TileMapGame
)
set_target_properties( TileMapGame
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}../../../Build"
)

#Custom Commands
add_custom_command(
            TARGET TileMapGame 
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}../../../AddToBuild/Config ${CMAKE_BINARY_DIR}/Config)
add_custom_command(
            TARGET TileMapGame 
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}../../../AddToBuild/Packages ${CMAKE_BINARY_DIR}/Packages)
add_custom_command(
            TARGET TileMapGame 
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}../../../AddToBuild/Config ${CMAKE_BINARY_DIR}../../../Build/$<CONFIGURATION>/Config)
add_custom_command(
            TARGET TileMapGame 
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}../../../AddToBuild/Packages ${CMAKE_BINARY_DIR}../../../Build/$<CONFIGURATION>/Packages)
add_custom_command(
            TARGET TileMapGame 
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}../../../Build/$<CONFIGURATION>/$<TARGET_FILE_NAME:TileMapGame> ${CMAKE_BINARY_DIR}/TileMapGame_temp.dll)