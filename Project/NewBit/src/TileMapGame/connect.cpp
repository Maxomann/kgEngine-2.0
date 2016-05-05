#include "stdafx.h"

#include "GraphicsComponent.h"
#include "TransformationComponent.h"
#include "AnimationComponent.h"

#include "ChunkSystem.h"
#include "GraphicsSystem.h"

#include "DefaultGameState.h"
#include "SingleplayerGameState.h"
#include "ConsoleGameState.h"

using namespace std;
using namespace kg;

void testfunc()
{ }

extern "C"
{
	DLL_EXPORT void kgConnect( PluginManager& pluginManager )
	{
		//testfunc();

		//Components
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<Component, TransformationComponent>>(
				( int )id::ComponentPluginId::TRANSFORMATION,
				TransformationComponent::PLUGIN_NAME ) );
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<Component, GraphicsComponent>>(
				( int )id::ComponentPluginId::GRAPHICS,
				GraphicsComponent::PLUGIN_NAME ) );
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<Component, AnimationComponent>>(
				( int )id::ComponentPluginId::ANIMATION,
				AnimationComponent::PLUGIN_NAME ) );
		//Camera not registered as a plugin. A camera entity is created in the GraphicsSystem

		//Systems
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<System, ChunkSystem>>(
				( int )id::SystemPluginId::CHUNK_SYSTEM,
				ChunkSystem::PLUGIN_NAME ) );
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<System, GraphicsSystem>>(
				( int )id::SystemPluginId::GRAPHICS_SYSTEM,
				GraphicsSystem::PLUGIN_NAME ) );
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<System, ChunkGeneratorSystem>>(
				( int )id::SystemPluginId::CHUNK_GENERATOR,
				ChunkGeneratorSystem::PLUGIN_NAME ) );
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<System, AnimationSystem>>(
				( int )id::SystemPluginId::ANIMATION_SYSTEM,
				AnimationSystem::PLUGIN_NAME ) );

		//GameStates
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<GameState, DefaultGameState>>(
				( int )id::DEFAULT_GAMESTATE_ID,
				DefaultGameState::PLUGIN_NAME ) );
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<GameState, SingleplayerGameState>>(
				( int )id::GameStatePluginId::SINGLEPLAYER,
				SingleplayerGameState::PLUGIN_NAME ) );
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<GameState, ConsoleGameState>>(
				( int )id::GameStatePluginId::CONSOLE,
				ConsoleGameState::PLUGIN_NAME ) );

		//AnimationHandlers
		pluginManager.addPluginFactory(
			std::make_shared<PluginFactory<AnimationHandler, EasyAnimationHandler>>(
				( int )id::AnimationHandlerPluginId::ANIMATION_HANDLER_EASY,
				EasyAnimationHandler::PLUGIN_NAME ) );
	}
}