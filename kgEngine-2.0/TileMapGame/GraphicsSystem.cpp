#include "GraphicsSystem.h"
using namespace std;
using namespace sf;

namespace kg
{

	void GraphicsSystem::init( Engine& engine, World& world, std::shared_ptr<ConfigFile>& configFile )
	{
		m_configFile = configFile;

		engine.renderWindow.create( sf::VideoMode( 1080, 720 ), "Test" );
		engine.renderWindow.setVerticalSyncEnabled( true );

		m_cameras.push_back( Camera::CREATE( engine, world ) );
	}

	void GraphicsSystem::sfmlEvent( Engine& engine, const sf::Event& sfEvent )
	{
		return;
	}

	void GraphicsSystem::update( Engine& engine, World& world )
	{
		return;
	}

	double GraphicsSystem::getUpdateImportance() const
	{
		return ( double )id::SystemUpdateImportance::GRAPHICS_SYSTEM;
	}

	const std::string& GraphicsSystem::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	int GraphicsSystem::getPluginId() const
	{
		return ( int )id::SystemUpdateImportance::GRAPHICS_SYSTEM;
	}

	std::shared_ptr<Entity>& GraphicsSystem::getCamera( int index )
	{
		return m_cameras.at( index );
	}

	const std::string GraphicsSystem::PLUGIN_NAME="GraphicsSystem";
}
