#include "GraphicsSystem.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GraphicsSystem::init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile )
	{
		m_configFile = configFile;

		m_connectToSignal( saveManager.s_savegameOpened, &GraphicsSystem::m_onSavegameOpened );

		//get config values
		m_configValues.antialiasing = configFile->getData( ANTIALIASING );
		m_configValues.fullscreen = configFile->getData( FULLSCREEN );
		m_configValues.window_resx = configFile->getData( WINDOW_RESX );
		m_configValues.window_resy = configFile->getData( WINDOW_RESY );
		m_configValues.render_resx = configFile->getData( RENDER_RESX );
		m_configValues.render_resy = configFile->getData( RENDER_RESY );
		m_configValues.vsync = configFile->getData( VSYNC );
		m_configValues.window_name = configFile->getData( WINDOW_NAME );

		//set them if invalid ( and retrieve them a second time )
		if( !m_configValues.antialiasing )
			m_configValues.antialiasing = configFile->setData( ANTIALIASING, ANTIALIASING_DEFAULT );
		if( !m_configValues.fullscreen )
			m_configValues.fullscreen = configFile->setData( FULLSCREEN, FULLSCREEN_DEFAULT );
		if( !m_configValues.window_resx )
			m_configValues.window_resx = configFile->setData( WINDOW_RESX, WINDOW_RESX_DEFAULT );
		if( !m_configValues.window_resy )
			m_configValues.window_resy = configFile->setData( WINDOW_RESY, WINDOW_RESY_DEFAULT );
		if( !m_configValues.render_resx )
			m_configValues.render_resx = configFile->setData( RENDER_RESX, RENDER_RESX_DEFAULT );
		if( !m_configValues.render_resy )
			m_configValues.render_resy = configFile->setData( RENDER_RESY, RENDER_RESY_DEFAULT );
		if( !m_configValues.vsync )
			m_configValues.vsync = configFile->setData( VSYNC, VSYNC_DEFAULT );
		if( !m_configValues.window_name )
			m_configValues.window_name = configFile->setData( WINDOW_NAME, WINDOW_NAME_DEFAULT );

		//init window
		sf::ContextSettings contextSettings;
		contextSettings.antialiasingLevel = m_configValues.antialiasing.toInt();

		if( m_configValues.fullscreen.toBool() )
		{
			//fullscreen
			engine.renderWindow.create(
				sf::VideoMode::getDesktopMode(),//ignores: window_resx, window_resy
				m_configValues.window_name.toString(),
				sf::Style::Fullscreen,
				contextSettings );
		}
		else
		{
			//no fullscreen
			engine.renderWindow.create(
				sf::VideoMode( m_configValues.window_resx.toInt(), m_configValues.window_resy.toInt(), 32 ),
				m_configValues.window_name.toString(),
				sf::Style::Close,
				contextSettings );
		}
		engine.renderWindow.setVerticalSyncEnabled( m_configValues.vsync.toBool() );

		m_initCameras( engine, world );

	}

	void GraphicsSystem::sfmlEvent( Engine& engine, const sf::Event& sfEvent )
	{
		return;
	}

	void GraphicsSystem::update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime )
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

	Plugin::Id GraphicsSystem::getPluginId() const
{
		return ( int )id::SystemUpdateImportance::GRAPHICS_SYSTEM;
	}

	std::shared_ptr<Entity>& GraphicsSystem::getCamera( int index )
	{
		return m_cameras.at( index );
	}

	void GraphicsSystem::m_onSavegameOpened( Engine& engine, World& world )
	{
		m_cameras.clear();
		m_initCameras( engine, world );
	}

	void GraphicsSystem::m_initCameras( Engine& engine, World& world )
	{
		//init camera
		auto camera = Camera::EMPLACE_TO_WORLD( engine, world );
		if( m_configValues.fullscreen.toBool() )
		{
			//fullscreen
			camera->getComponent<Camera>()->setRenderResolution(
				sf::Vector2u( m_configValues.render_resx.toInt(),
				m_configValues.render_resy.toInt() ) );
		}
		else
		{
			//no fullscreen
			//ignores: render_resx, render_resy
		}
		m_cameras.push_back( Camera::EMPLACE_TO_WORLD( engine, world ) );
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
