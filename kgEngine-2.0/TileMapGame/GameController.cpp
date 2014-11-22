#include "GameController.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GameController::init( Engine& engine, World& world )
	{
		m_test = world.addEntity( world.createEntity( engine, 100 ) ).second;
		m_camera = Camera::CREATE( engine, world );
		return;
	}

	void GameController::sfmlEvent( const sf::Event& sfEvent )
	{
		return;
	}

	void GameController::update( Engine& engine, World& world )
	{
		if( Keyboard::isKeyPressed( Keyboard::W ) )
			m_camera->getComponent<Position>()->move( sf::Vector2i( 0, -10 ) );
		if( Keyboard::isKeyPressed( Keyboard::S ) )
			m_camera->getComponent<Position>()->move( sf::Vector2i( 0, 10 ) );
		if( Keyboard::isKeyPressed( Keyboard::A ) )
			m_camera->getComponent<Position>()->move( sf::Vector2i( -10, 0 ) );
		if( Keyboard::isKeyPressed( Keyboard::D ) )
			m_camera->getComponent<Position>()->move( sf::Vector2i( 10, 0 ) );

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

	const std::string GameController::PLUGIN_NAME="GAME_CORE";
}
