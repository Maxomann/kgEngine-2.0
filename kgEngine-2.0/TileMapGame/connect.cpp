#include<_PluginCreator.h>

#include "Position.h"
#include "Size.h"
#include "Graphics.h"
#include "Rotation.h"
#include "GlobalBounds.h"

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
			std::make_shared<PluginFactory<Component, Position>>(
			( int )id::ComponentPluginId::POSITION,
			Position::PLUGIN_NAME ) );
		pluginManager.addComponentPlugin(
			std::make_shared<PluginFactory<Component, Size>>(
			( int )id::ComponentPluginId::SIZE,
			Size::PLUGIN_NAME ) );
		pluginManager.addComponentPlugin(
			std::make_shared<PluginFactory<Component, Graphics>>(
			( int )id::ComponentPluginId::GRAPHICS,
			Graphics::PLUGIN_NAME ) );
		pluginManager.addComponentPlugin(
			std::make_shared<PluginFactory<Component, Rotation>>(
			( int )id::ComponentPluginId::ROTATION,
			Rotation::PLUGIN_NAME ) );
		pluginManager.addComponentPlugin(
			std::make_shared<PluginFactory<Component, GlobalBounds>>(
			( int )id::ComponentPluginId::GLOBAL_BOUNDS,
			GlobalBounds::PLUGIN_NAME ) );
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
	}
}