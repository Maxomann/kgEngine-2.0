#include "GraphicsSystem.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GraphicsSystem::init( Engine& engine, World& world, std::shared_ptr<ConfigFile>& configFile )
	{
		m_configFile = configFile;

		//get config values
		auto antialiasing = configFile->getData( ANTIALIASING );
		auto fullscreen = configFile->getData( FULLSCREEN );
		auto window_resx = configFile->getData( WINDOW_RESX );
		auto window_resy = configFile->getData( WINDOW_RESY );
		auto render_resx = configFile->getData( RENDER_RESX );
		auto render_resy = configFile->getData( RENDER_RESY );
		auto vsync = configFile->getData( VSYNC );
		auto window_name = configFile->getData( WINDOW_NAME );

		//set them if invalid ( and retrieve them a second time )
		if( !antialiasing )
			antialiasing = configFile->setData( ANTIALIASING, ANTIALIASING_DEFAULT );
		if( !fullscreen )
			fullscreen = configFile->setData( FULLSCREEN, FULLSCREEN_DEFAULT );
		if( !window_resx )
			window_resx = configFile->setData( WINDOW_RESX, WINDOW_RESX_DEFAULT );
		if( !window_resy )
			window_resy = configFile->setData( WINDOW_RESY, WINDOW_RESY_DEFAULT );
		if( !render_resx )
			render_resx = configFile->setData( RENDER_RESX, RENDER_RESX_DEFAULT );
		if( !render_resy )
			render_resy = configFile->setData( RENDER_RESY, RENDER_RESY_DEFAULT );
		if( !vsync )
			vsync = configFile->setData( VSYNC, VSYNC_DEFAULT );
		if( !window_name )
			window_name = configFile->setData( WINDOW_NAME, WINDOW_NAME_DEFAULT );

		//init window
		sf::ContextSettings contextSettings;
		contextSettings.antialiasingLevel = antialiasing.toInt();

		if( fullscreen.toBool() )
		{
			//fullscreen
			engine.renderWindow.create(
				sf::VideoMode::getDesktopMode(),//ignores: window_resx, window_resy
				window_name.toString(),
				sf::Style::Fullscreen,
				contextSettings );
		}
		else
		{
			//no fullscreen
			engine.renderWindow.create(
				sf::VideoMode( window_resx.toInt(), window_resy.toInt(), 32 ),
				window_name.toString(),
				sf::Style::Close,
				contextSettings );
		}
		engine.renderWindow.setVerticalSyncEnabled( vsync.toBool() );

		//init camera
		auto camera = Camera::CREATE( engine, world );
		if( fullscreen.toBool() )
		{
			//fullscreen
			camera->getComponent<Camera>()->setRenderResolution( sf::Vector2u( render_resx.toInt(), render_resy.toInt() ) );
		}
		else
		{
			//no fullscreen
			//ignores: render_resx, render_resy
		}
		m_cameras.push_back( camera );
	}

	void GraphicsSystem::sfmlEvent( Engine& engine, const sf::Event& sfEvent )
	{
		return;
	}

	void GraphicsSystem::update( Engine& engine, World& world, const sf::Time& frameTime )
	{
		for( const auto& el : m_cameras )
			engine.renderWindow.draw( *el->getComponent<Camera>() );
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

	const std::string GraphicsSystem::WINDOW_NAME_DEFAULT = "DefaultWindowName";

	const std::string GraphicsSystem::VSYNC_DEFAULT = "true";

	const std::string GraphicsSystem::RENDER_RESY_DEFAULT = "1080px";

	const std::string GraphicsSystem::RENDER_RESX_DEFAULT = "1920px";

	const std::string GraphicsSystem::WINDOW_RESY_DEFAULT = "720px";

	const std::string GraphicsSystem::WINDOW_RESX_DEFAULT = "1080px";

	const std::string GraphicsSystem::FULLSCREEN_DEFAULT = "false";

	const std::string GraphicsSystem::ANTIALIASING_DEFAULT = "0";

	const std::string GraphicsSystem::ANTIALIASING = "iAntialiasing";

	const std::string GraphicsSystem::VSYNC = "bVsync";

	const std::string GraphicsSystem::RENDER_RESY = "iRender_resy";

	const std::string GraphicsSystem::RENDER_RESX = "iRender_resx";

	const std::string GraphicsSystem::WINDOW_RESY = "iWindow_resy";

	const std::string GraphicsSystem::WINDOW_RESX = "iWindow_resx";

	const std::string GraphicsSystem::FULLSCREEN = "bFullscreen";

	const std::string GraphicsSystem::WINDOW_NAME = "sWindow_name";

	const std::string GraphicsSystem::PLUGIN_NAME = "GraphicsSystem";
}
