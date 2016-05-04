#include "DefaultGameState.h"
using namespace std;
using namespace sf;

namespace kg
{
	void DefaultGameState::onInit()
	{
		unique_ptr<GameState> gameStatePtr = r_engine->pluginManager.createPlugin<GameState>( ( int )id::GameStatePluginId::SINGLEPLAYER );
		r_gameStateManager->push( move( gameStatePtr ) );
	}

	void DefaultGameState::registerGui( tgui::Gui& gui )
	{
		return;
	}

	void DefaultGameState::registerInputCallbacks( InputManager& inputManager )
	{
		Action eventClose( Event::Closed );
		Action escapePress( Keyboard::Escape, Action::PressOnce );

		Action strgRightPress( Keyboard::RControl, Action::PressOnce );

		inputManager.setAction( id::Input::SHUT_DOWN,
								escapePress || eventClose,
								bind( &DefaultGameState::shutDown, this ) );

		inputManager.setAction( id::Input::SWITCH_CONSOLE, strgRightPress,
								bind( &DefaultGameState::switchConsole, this ) );
	}

	void DefaultGameState::onUpdate()
	{ }

	void DefaultGameState::removeInputCallbacks( InputManager& inputManager )
	{
		inputManager.removeAction( id::Input::SHUT_DOWN );

		inputManager.removeAction( id::Input::SWITCH_CONSOLE );
	}

	void DefaultGameState::removeGui( tgui::Gui& gui )
	{
		return;
	}

	void DefaultGameState::onDestroy()
	{
		return;
	}

	const Plugin::Name& DefaultGameState::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id DefaultGameState::getPluginId() const
	{
		return id::DEFAULT_GAMESTATE_ID;
	}

	const std::string DefaultGameState::PLUGIN_NAME = "GameState Default";

	void DefaultGameState::saveOpenSavegame()
	{
		r_world->getSystem<ChunkSystem>()->saveAllLoadedChunks( *r_engine, *r_world, *r_saveManager );
		r_saveManager->saveSystems( *r_world );
	}

	void DefaultGameState::shutDown()
	{
		saveOpenSavegame();
		r_saveManager->closeSavegame( *r_engine, *r_world );
		r_engine->shouldTerminate = true;
	}

	void DefaultGameState::switchConsole()
	{
		const auto& id_console = id::GameStatePluginId::CONSOLE;

		if( r_gameStateManager->hasAnyInstanceOf( id_console ) )
		{
			r_gameStateManager->removeAllInstancesOf( id_console );
		}
		else
		{
			unique_ptr<GameState> gameStatePtr = r_engine->pluginManager.createPlugin<GameState>( ( int )id::GameStatePluginId::CONSOLE );
			r_gameStateManager->push( move( gameStatePtr ) );
		}
	}
}