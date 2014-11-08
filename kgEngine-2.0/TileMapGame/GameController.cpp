#include "GameController.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GameController::init( Engine& engine, World& world )
	{
		m_test = world.addEntity( world.createEntity( engine, 100 ) ).second;
		m_camera = Camera::emplaceToWorld( engine, world );
		return;
	}

	void GameController::sfmlEvent( const sf::Event& sfEvent )
	{
		return;
	}

	void GameController::update( Engine& engine, World& world )
	{
		return;
	}

	double GameController::getUpdateImportance() const
	{
		return ( double )id::SystemUpdateImportance::GAME;
	}

	const std::string& GameController::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	int GameController::getPluginId() const
	{
		return ( int )id::SystemPluginId::GAME;
	}

	const std::string GameController::PLUGIN_NAME="GAME_CORE";
}
