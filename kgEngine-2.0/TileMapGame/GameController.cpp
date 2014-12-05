#include "GameController.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GameController::init( Engine& engine, World& world )
	{
		m_test = world.addEntity( world.createEntity( engine, 100 ) ).second;

		for( int x = 0; x < 10; ++x )
			for( int y = 0; y < 10; ++y )
			{
				auto entity = world.addEntity( world.createEntity( engine, 100 ) ).second;
				entity->getComponent<Position>()->set( sf::Vector2i( x * 64, y * 64 ) );
			}

		m_camera = Camera::CREATE( engine, world );
		m_camera->getComponent<Size>()->set( sf::Vector2i( 1080, 720 ) );
		return;
	}

	void GameController::sfmlEvent( Engine& engine, const sf::Event& sfEvent )
	{
		if( sfEvent.type == Event::Closed )
			engine.shouldTerminate = true;
		return;
	}

	void GameController::update( Engine& engine, World& world )
	{
		if( Keyboard::isKeyPressed( Keyboard::Escape ) )
			engine.shouldTerminate = true;
		if( Keyboard::isKeyPressed( Keyboard::W ) )
			m_camera->getComponent<Position>()->move( sf::Vector2i( 0, -10 ) );
		if( Keyboard::isKeyPressed( Keyboard::S ) )
			m_camera->getComponent<Position>()->move( sf::Vector2i( 0, 10 ) );
		if( Keyboard::isKeyPressed( Keyboard::A ) )
			m_camera->getComponent<Position>()->move( sf::Vector2i( -10, 0 ) );
		if( Keyboard::isKeyPressed( Keyboard::D ) )
			m_camera->getComponent<Position>()->move( sf::Vector2i( 10, 0 ) );

		auto chunk = world.getSystem<ChunkSystem>()->getChunkOfEntity( m_camera );

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

	int GameController::getPluginId() const
	{
		return ( int )id::SystemPluginId::GAME_CONTROLLER;
	}

	const std::string GameController::PLUGIN_NAME = "GAME_CORE";
}