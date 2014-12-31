#include "GameController.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GameController::init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile )
	{
		r_graphicsSystem = world.getSystem<GraphicsSystem>().get();

		saveManager.openSavegame( engine, world, "MyFirstSavegameEver" );
		saveManager.loadEntitiesFromFile( engine, world, "EntitiesInHere" );

		/*for( int x = 0; x < 10; ++x )
			for( int y = 0; y < 10; ++y )
			{
				auto entity = world.addEntity( world.createNewSaveableEntity( engine, 100 ) ).second;
				entity->getComponent<Position>()->set( sf::Vector2i( x * 64, y * 64 ) );
			}*/

		return;
	}

	void GameController::sfmlEvent( Engine& engine, const sf::Event& sfEvent )
	{
		if( sfEvent.type == Event::Closed )
			engine.shouldTerminate = true;
		return;
	}

	void GameController::update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime )
	{
		auto camera = r_graphicsSystem->getCamera( 0 );

		if( Keyboard::isKeyPressed( Keyboard::Escape ) )
			engine.shouldTerminate = true;
		if( !engine.isPaused )
		{
			if( Keyboard::isKeyPressed( Keyboard::W ) )
				camera->getComponent<Position>()->move( sf::Vector2i( 0, -10 ) );
			if( Keyboard::isKeyPressed( Keyboard::S ) )
				camera->getComponent<Position>()->move( sf::Vector2i( 0, 10 ) );
			if( Keyboard::isKeyPressed( Keyboard::A ) )
				camera->getComponent<Position>()->move( sf::Vector2i( -10, 0 ) );
			if( Keyboard::isKeyPressed( Keyboard::D ) )
				camera->getComponent<Position>()->move( sf::Vector2i( 10, 0 ) );
		}
		if( Keyboard::isKeyPressed( Keyboard::F5 ) )
		{
			saveManager.saveSystems( world );
			saveManager.saveEntitiesToFile( "EntitiesInHere", world.getAllEntities() );

			saveManager.openSavegame( engine, world, "MyFirstSavegameEver" );
			saveManager.loadEntitiesFromFile( engine, world, "EntitiesInHere" );
		}

		return;
	}

	double GameController::getUpdateImportance() const
	{
		return ( double )id::SystemUpdateImportance::GAME_CONTROLLER;
	}

	const std::string& GameController::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id GameController::getPluginId() const
{
		return ( int )id::SystemPluginId::GAME_CONTROLLER;
	}

	const std::string GameController::PLUGIN_NAME = "GameControllerSystem";
}
