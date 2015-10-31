#include "GameState.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GameState::initReferences( Engine& engine, World& world, SaveManager& saveManager, GameStateManager& gameStateManager )
	{
		r_engine = &engine;
		r_world = &world;
		r_saveManager = &saveManager;
		r_gameStateManager = &gameStateManager;
	}

	void GameState::updateFrameTime( const sf::Time& frameTime )
	{
		this->frameTime = frameTime;
	}
}