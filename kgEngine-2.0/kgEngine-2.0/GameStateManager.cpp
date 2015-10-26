#include "GameStateManager.h"
using namespace std;
using namespace sf;

namespace kg
{
	
	GameStateManager::GameStateManager( Engine& engine, World& world, SaveManager& saveManager )
		: engine(engine),
		world(world),
		saveManager(saveManager)
	{

	}

	void GameStateManager::init()
	{
		auto defaultGameState = engine.pluginManager.createPlugin<GameState>( id::DEFAULT_GAMESTATE_ID );

		push( defaultGameState );
	}

	void GameStateManager::forwardFrameTime( const sf::Time& frameTime )
	{
		for( auto& el : m_gameStateStack )
			el->updateFrameTime( frameTime );
	}

	void GameStateManager::push( std::unique_ptr<GameState>& gameState )
	{
		gameState->initReferences( engine, world, saveManager, *this );

		gameState->registerGui( engine.inputManager.gui );
		gameState->registerInputCallbacks( engine.inputManager );
		gameState->onInit();

		m_gameStateStack.push_back( std::move(gameState) );
	}

	void GameStateManager::pop()
	{
		auto& gameState = m_gameStateStack.back();

		gameState->onDestroy();
		gameState->removeInputCallbacks( engine.inputManager );
		gameState->removeGui( engine.inputManager.gui );

		m_gameStateStack.pop_back();
	}

	const std::unique_ptr<GameState>& GameStateManager::top() const
	{
		return m_gameStateStack.back();
	}

	void GameStateManager::onUpdate()
	{
		for( auto& ptr : m_gameStateStack )
			ptr->onUpdate();
	}

	bool GameStateManager::hasAnyInstanceOf( const Plugin::Id& pluginId )
	{
		for( const auto& el : m_gameStateStack )
			if( el->getPluginId() == pluginId )
				return true;
		return false;
	}

	void GameStateManager::removeAllInstancesOf( const Plugin::Id& pluginId )
	{
		m_gameStateStack.erase(remove_if( m_gameStateStack.begin(), m_gameStateStack.end(), [&]( unique_ptr<GameState>& el )
		{
			if( el->getPluginId() == pluginId )
			{
				el->removeInputCallbacks( engine.inputManager );
				el->removeGui( engine.inputManager.gui );
				el->onDestroy();
				return true;
			}
			else
				return false;
		} ));
	}

	void GameStateManager::clear()
	{
		while( m_gameStateStack.size() != 0 )
			pop();
	}

}
