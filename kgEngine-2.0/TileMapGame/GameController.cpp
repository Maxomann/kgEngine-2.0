#include "GameController.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GameController::init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile )
	{
		r_graphicsSystem = world.getSystem<GraphicsSystem>();

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
		if( m_camera.expired() )
			m_camera = r_graphicsSystem->getCamera( 0 );
		auto camera = m_camera.lock();

		auto frameTimeInMilliseconds = frameTime.asMilliseconds();

		if( Keyboard::isKeyPressed( Keyboard::Escape ) )
			shutDown( engine, world, saveManager );
		if( !engine.isPaused )
		{
			Vector2i cameraMovement;

			if( Keyboard::isKeyPressed( Keyboard::W ) )
				cameraMovement += sf::Vector2i( 0, -10.0 / 16.0 * frameTimeInMilliseconds );
			if( Keyboard::isKeyPressed( Keyboard::S ) )
				cameraMovement += sf::Vector2i( 0, 10.0 / 16.0 * frameTimeInMilliseconds );
			if( Keyboard::isKeyPressed( Keyboard::A ) )
				cameraMovement += sf::Vector2i( -10.0 / 16.0 * frameTimeInMilliseconds, 0 );
			if( Keyboard::isKeyPressed( Keyboard::D ) )
				cameraMovement += sf::Vector2i( 10.0 / 16.0 * frameTimeInMilliseconds, 0 );
			if( Keyboard::isKeyPressed( Keyboard::Add ) )
				m_cameraZoomFactor -= 0.01*frameTimeInMilliseconds;
			if( Keyboard::isKeyPressed( Keyboard::Subtract ) )
				m_cameraZoomFactor += 0.01*frameTimeInMilliseconds;

			if( Keyboard::isKeyPressed( Keyboard::LControl ) )
				cameraMovement /= 2;
			else if( Keyboard::isKeyPressed( Keyboard::Space ) )
				cameraMovement *= 4;

			camera->getComponent<Transformation>()->move( cameraMovement );
		}
		if( m_cameraZoomFactor < 0 )
			m_cameraZoomFactor = 0;
		camera->getComponent<Transformation>()->setSize( Vector2i( (double)1280.0 * m_cameraZoomFactor, (double)720.0 * m_cameraZoomFactor ) );

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

	const size_t& GameController::getRTTI_hash() const
	{
		return type_hash;
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

	const size_t GameController::type_hash = getRuntimeTypeInfo<GameController>();

}