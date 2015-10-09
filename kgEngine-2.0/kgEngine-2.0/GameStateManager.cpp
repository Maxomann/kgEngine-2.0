#include "GameStateManager.h"
using namespace std;
using namespace sf;
using namespace thor;

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
		auto defaultGameState = engine.pluginManager.createPlugin<GameState>( 0 );

		push( defaultGameState );
	}

	void GameStateManager::push( std::shared_ptr<GameState>& gameState )
	{
		gameState->initReferences( engine, world, saveManager );
		m_gameStateStack.push_back( gameState );
	}

	void GameStateManager::pop()
	{
		m_gameStateStack.pop_back();
	}

	const std::shared_ptr<GameState>& GameStateManager::top() const
	{
		return m_gameStateStack.back();
	}

	void GameStateManager::onUpdate()
	{
		for( auto& el : m_gameStateStack )
			el->onUpdate(*this);
	}

}
