#include "GameController.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GameController::init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile )
	{
		r_graphicsSystem = world.getSystem<GraphicsSystem>().get();

		saveManager.openSavegame( engine, world, "MyFirstSavegameEver" );
		//saveManager.loadEntitiesFromFile( engine, world, "EntitiesInHere" );

		/*int fieldSize = 10;
		for( int x = 0; x < fieldSize; ++x )
		for( int y = 0; y < fieldSize; ++y )
		{
		auto entity = world.addEntity( world.createNewSaveableEntity( engine, 100 ) ).second;
		entity->getComponent<Transformation>()->setPosition( sf::Vector2i( x * 64, y * 64 ) );
		}*/

		return;
	}

	void GameController::sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent )
	{
		if( sfEvent.type == Event::Closed )
			shutDown( engine, world, saveManager );
		return;
	}

	void GameController::update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime )
	{
		auto camera = r_graphicsSystem->getCamera( 0 );

		auto frameTimeInMilliseconds = frameTime.asMilliseconds();

		if( Keyboard::isKeyPressed( Keyboard::Escape ) )
			shutDown( engine, world, saveManager );
		if( !engine.isPaused )
		{
			if( Keyboard::isKeyPressed( Keyboard::W ) )
				camera->getComponent<Transformation>()->move( sf::Vector2i( 0, -10.0 / 16.0 * frameTimeInMilliseconds ) );
			if( Keyboard::isKeyPressed( Keyboard::S ) )
				camera->getComponent<Transformation>()->move( sf::Vector2i( 0, 10.0 / 16.0 * frameTimeInMilliseconds ) );
			if( Keyboard::isKeyPressed( Keyboard::A ) )
				camera->getComponent<Transformation>()->move( sf::Vector2i( -10.0 / 16.0 * frameTimeInMilliseconds, 0 ) );
			if( Keyboard::isKeyPressed( Keyboard::D ) )
				camera->getComponent<Transformation>()->move( sf::Vector2i( 10.0 / 16.0 * frameTimeInMilliseconds, 0 ) );
			if( Keyboard::isKeyPressed( Keyboard::Add ) )
				m_cameraZoomFactor -= 0.01*frameTimeInMilliseconds;
			if( Keyboard::isKeyPressed( Keyboard::Subtract ) )
				m_cameraZoomFactor += 0.01*frameTimeInMilliseconds;
		}
		if( m_cameraZoomFactor < 0 )
			m_cameraZoomFactor = 0;
		camera->getComponent<Transformation>()->setSize( Vector2i( 1080.0 * m_cameraZoomFactor, 720.0 * m_cameraZoomFactor ) );

		if( Keyboard::isKeyPressed( Keyboard::F5 ) )
		{
			saveOpenSavegame( engine, world, saveManager );

			saveManager.openSavegame( engine, world, "MyFirstSavegameEver" );
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

	void GameController::shutDown( Engine& engine, World& world, SaveManager& saveManager )
	{
		saveOpenSavegame( engine, world, saveManager );
		engine.shouldTerminate = true;
	}

	void GameController::saveOpenSavegame( Engine& engine, World& world, SaveManager& saveManager )
	{
		world.getSystem<ChunkSystem>()->saveAllLoadedChunks( engine, world, saveManager );
		saveManager.saveSystems( world );
	}

	const std::string GameController::PLUGIN_NAME = "GameControllerSystem";
}