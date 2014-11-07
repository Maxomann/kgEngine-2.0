#include "Game.h"
using namespace std;
using namespace sf;

namespace kg
{
	void Game::init( Engine& engine, World& world )
	{
		m_test = world.addEntity( world.createEntity( engine, 100 ) ).second;
		return;
	}

	void Game::sfmlEvent( const sf::Event& sfEvent )
	{
		return;
	}

	void Game::update( Engine& engine, World& world )
	{
		return;
	}

	double Game::getUpdateImportance() const
	{
		return ( double )id::SystemUpdateImportance::GAME;
	}

	const std::string& Game::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	int Game::getPluginId() const
	{
		return ( int )id::SystemPluginId::GAME;
	}

	const std::string Game::PLUGIN_NAME="GAME_CORE";
}
