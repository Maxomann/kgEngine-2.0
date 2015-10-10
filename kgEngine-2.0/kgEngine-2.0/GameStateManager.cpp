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

	void GameStateManager::push( const std::shared_ptr<GameState>& gameState )
	{
		gameState->initReferences( engine, world, saveManager );

		gameState->onInit();
		gameState->registerGui( engine.inputManager.gui );
		gameState->registerInputCallbacks( engine.inputManager );

		m_gameStateStack.push_back( gameState );
	}

	void GameStateManager::pop()
	{
		auto& gameState = m_gameStateStack.back();

		gameState->removeInputCallbacks( engine.inputManager );
		gameState->removeGui( engine.inputManager.gui );
		gameState->onDestroy();

		m_gameStateStack.pop_back();
	}

	const std::shared_ptr<GameState>& GameStateManager::top() const
	{
		return m_gameStateStack.back();
	}

	void GameStateManager::onUpdate()
	{
		vector<weak_ptr<GameState>> temp;
		for( auto& el : m_gameStateStack )
			temp.push_back( el );

		for( auto& ptr : temp )
		{
			if(auto sptr = ptr.lock())
				sptr->onUpdate( *this );
		}
	}
}
