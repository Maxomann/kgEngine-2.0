#include<_PluginCreator.h>

#include "Graphics.h"
#include "Transformation.h"

#include "GameController.h"
#include "ChunkSystem.h"
#include "GraphicsSystem.h"

using namespace std;
using namespace kg;

extern "C"
{
	DLL_EXPORT void kgConnect( PluginManager& pluginManager )
	{
		//Components
		pluginManager.addComponentPlugin(
			std::make_shared<PluginFactory<Component, Transformation>>(
			( int )id::ComponentPluginId::TRANSFORMATION,
			Transformation::PLUGIN_NAME ) );
		pluginManager.addComponentPlugin(
			std::make_shared<PluginFactory<Component, Graphics>>(
			( int )id::ComponentPluginId::GRAPHICS,
			Graphics::PLUGIN_NAME ) );
		//Camera not registered as a plugin. A camera entity is created in the GraphicsSystem

		//Systems
		pluginManager.addSystemPlugin(
			std::make_shared<PluginFactory<System, GameController>>(
			( int )id::SystemPluginId::GAME_CONTROLLER,
			GameController::PLUGIN_NAME ) );
		pluginManager.addSystemPlugin(
			std::make_shared<PluginFactory<System, ChunkSystem>>(
			( int )id::SystemPluginId::CHUNK_SYSTEM,
			ChunkSystem::PLUGIN_NAME ) );
		pluginManager.addSystemPlugin(
			std::make_shared<PluginFactory<System, GraphicsSystem>>(
			( int )id::SystemPluginId::GRAPHICS_SYSTEM,
			GraphicsSystem::PLUGIN_NAME ) );
		pluginManager.addSystemPlugin(
			std::make_shared<PluginFactory<System, ChunkGenerator>>(
			( int )id::SystemPluginId::CHUNK_GENERATOR,
			ChunkGenerator::PLUGIN_NAME ) );
	}
}