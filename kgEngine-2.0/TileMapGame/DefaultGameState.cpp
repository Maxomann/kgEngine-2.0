#include "DefaultGameState.h"
using namespace std;
using namespace sf;

namespace kg
{

	void DefaultGameState::onInit()
	{
		unique_ptr<GameState> gameStatePtr = std::make_unique<SingleplayerGameState>();
		r_gameStateManager->push( gameStatePtr );
	}

	void DefaultGameState::registerGui( tgui::Gui& gui )
	{
		return;
	}

	void DefaultGameState::registerInputCallbacks( InputManager& inputManager )
	{
		Action escapePress( Keyboard::Escape, Action::PressOnce );

		Action eventClose( Event::Closed );

		inputManager.setAction( id::Input::SHUT_DOWN,
								escapePress || eventClose,
								bind( &DefaultGameState::shutDown, this ) );
	}

	void DefaultGameState::onUpdate()
	{

	}

	void DefaultGameState::removeInputCallbacks( InputManager& inputManager )
	{
		inputManager.removeAction( id::Input::SHUT_DOWN );
	}

	void DefaultGameState::removeGui( tgui::Gui& gui )
	{
		return;
	}

	void DefaultGameState::onDestroy()
	{
		throw exception( "DefaultGameState should never be destroyed" );
	}


	const Plugin::Name& DefaultGameState::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id DefaultGameState::getPluginId() const
	{
		return id::DEFAULT_GAMESTATE_ID;
	}

	const std::string DefaultGameState::PLUGIN_NAME = "Default GameState";

	void DefaultGameState::shutDown()
	{
		r_world->getSystem<ChunkSystem>()->saveOpenSavegame( *r_engine, *r_world, *r_saveManager );
		r_engine->shouldTerminate = true;
	}

}
