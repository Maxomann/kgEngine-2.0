#include "DefaultGameState.h"
using namespace std;
using namespace sf;

namespace kg
{

	void DefaultGameState::onInit()
	{
		return;
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

	void DefaultGameState::onUpdate( GameStateManager& gameStateManager )
	{
		if( !m_singleplayerGameStateLaunched )
		{
			gameStateManager.push( std::make_shared<SingleplayerGameState>() );
			m_singleplayerGameStateLaunched = true;
		}
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
