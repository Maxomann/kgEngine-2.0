#include<_PluginCreator.h>

#include "Graphics.h"
#include "Transformation.h"
#include "AnimationComponent.h"

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
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<Component, Transformation>>(
				( int )id::ComponentPluginId::TRANSFORMATION,
				Transformation::PLUGIN_NAME ) );
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<Component, Graphics>>(
				( int )id::ComponentPluginId::GRAPHICS,
				Graphics::PLUGIN_NAME ) );
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<Component, AnimationComponent>>(
				( int )id::ComponentPluginId::ANIMATION,
				AnimationComponent::PLUGIN_NAME ) );
		//Camera not registered as a plugin. A camera entity is created in the GraphicsSystem

		//Systems
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<System, GameController>>(
				( int )id::SystemPluginId::GAME_CONTROLLER,
				GameController::PLUGIN_NAME ) );
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<System, ChunkSystem>>(
				( int )id::SystemPluginId::CHUNK_SYSTEM,
				ChunkSystem::PLUGIN_NAME ) );
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<System, GraphicsSystem>>(
				( int )id::SystemPluginId::GRAPHICS_SYSTEM,
				GraphicsSystem::PLUGIN_NAME ) );
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<System, ChunkGenerator>>(
				( int )id::SystemPluginId::CHUNK_GENERATOR,
				ChunkGenerator::PLUGIN_NAME ) );
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<System, AnimationSystem>>(
				( int )id::SystemPluginId::ANIMATION_SYSTEM,
				AnimationSystem::PLUGIN_NAME ) );


		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<AnimationHandler, EasyAnimationHandler>>(
				( int )id::SpecialPluginId::ANIMATION_HANDLER_EASY,
				EasyAnimationHandler::PLUGIN_NAME ) );
	}
}
