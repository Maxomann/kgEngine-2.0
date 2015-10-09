#include "GameState.h"
using namespace std;
using namespace sf;
using namespace thor;

namespace kg
{
	
	void GameState::initReferences( Engine& engine, World& world, SaveManager& saveManager )
	{
		r_engine = &engine;
		r_world = &world;
		r_saveManager = &saveManager;
	}

}
