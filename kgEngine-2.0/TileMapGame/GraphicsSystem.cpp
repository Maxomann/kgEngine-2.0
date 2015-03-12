#include "GraphicsSystem.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GraphicsSystem::init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile )
	{
		m_configFile = configFile;

		m_connectToSignal( saveManager.s_savegameOpened, &GraphicsSystem::m_onSavegameOpened );
		m_connectToSignal( saveManager.s_savegameClosed, &GraphicsSystem::m_onSavegameClosed );
		m_connectToSignal( world.s_entity_added, &GraphicsSystem::m_onEntityAddedToWorld );
		m_connectToSignal( world.s_entity_removed, &GraphicsSystem::m_onEntityRemovedFromWorld );

		//get config values
		m_configValues.antialiasing = &configFile->getData( ANTIALIASING );
		m_configValues.fullscreen = &configFile->getData( FULLSCREEN );
		m_configValues.window_resx = &configFile->getData( WINDOW_RESX );
		m_configValues.window_resy = &configFile->getData( WINDOW_RESY );
		m_configValues.render_resx = &configFile->getData( RENDER_RESX );
		m_configValues.render_resy = &configFile->getData( RENDER_RESY );
		m_configValues.vsync = &configFile->getData( VSYNC );
		m_configValues.window_name = &configFile->getData( WINDOW_NAME );

		//set them if invalid ( and retrieve them a second time )
		if( !m_configValues.antialiasing->size() )
			*m_configValues.antialiasing = ANTIALIASING_DEFAULT;
		if( !m_configValues.fullscreen->size() )
			*m_configValues.fullscreen = FULLSCREEN_DEFAULT;
		if( !m_configValues.window_resx->size() )
			*m_configValues.window_resx = WINDOW_RESX_DEFAULT;
		if( !m_configValues.window_resy->size() )
			*m_configValues.window_resy = WINDOW_RESY_DEFAULT;
		if( !m_configValues.render_resx->size() )
			*m_configValues.render_resx = RENDER_RESX_DEFAULT;
		if( !m_configValues.render_resy->size() )
			*m_configValues.render_resy = RENDER_RESY_DEFAULT;
		if( !m_configValues.vsync->size() )
			*m_configValues.vsync = VSYNC_DEFAULT;
		if( !m_configValues.window_name->size() )
			*m_configValues.window_name = WINDOW_NAME_DEFAULT;

		//init window
		sf::ContextSettings contextSettings;
		contextSettings.antialiasingLevel = boost::lexical_cast< int >(*m_configValues.antialiasing);

		if( boost::lexical_cast< bool >(*m_configValues.fullscreen) )
		{
			//fullscreen
			engine.renderWindow.create(
				sf::VideoMode::getDesktopMode(),//ignores: window_resx, window_resy
				*m_configValues.window_name,
				sf::Style::Fullscreen,
				contextSettings );
		}
		else
		{
			//no fullscreen
			engine.renderWindow.create(
				sf::VideoMode(
				boost::lexical_cast< int >(*m_configValues.window_resx),
				boost::lexical_cast< int >(*m_configValues.window_resy), 32 ),
				*m_configValues.window_name,
				sf::Style::Close,
				contextSettings );
		}
		engine.renderWindow.setVerticalSyncEnabled( boost::lexical_cast< bool >(*m_configValues.vsync) );
		engine.renderWindow.setActive( false );

		m_launchDrawingThread( engine.renderWindow );
	}

	void GraphicsSystem::sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent )
	{
		return;
	}

	void GraphicsSystem::update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime )
	{
		if( m_shouldInitCameras )
		{
			m_initCameras( engine, world );
			m_shouldInitCameras = false;
		}


		engine.renderWindow.setTitle( *m_configValues.window_name +
									  " " +
									  to_string( frameTime.asMilliseconds() ) +
									  " :: " +
									  to_string( m_drawingThreadFrameTime ) +
									  " EntityCount: " +
									  to_string( world.getEntityCount() ) );

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

	std::shared_ptr<Entity> GraphicsSystem::getCamera( int index )
	{
		m_cameraContainerMutex.lock();
		auto retVal = m_cameras.at( index );
		m_cameraContainerMutex.unlock();
		return retVal;
	}

	void GraphicsSystem::m_onSavegameOpened( Engine& engine )
	{
		m_shouldInitCameras = true;
	}

	void GraphicsSystem::m_onSavegameClosed()
	{
		m_cameraContainerMutex.lock();
		m_cameras.clear();
		m_cameraContainerMutex.unlock();
	}

	void GraphicsSystem::m_onEntityAddedToWorld( const std::shared_ptr<Entity>& entity )
	{
		m_drawableEntityMutex.lock();
		if( entity->hasComponent<Graphics>() )
			m_drawableEntities.insert( entity );
		m_drawableEntityMutex.unlock();
	}

	void GraphicsSystem::m_onEntityRemovedFromWorld( const std::shared_ptr<Entity>& entity )
	{
		m_drawableEntityMutex.lock();
		if( entity->hasComponent<Graphics>() )
			m_drawableEntities.erase( entity );
		m_drawableEntityMutex.unlock();
	}

	void GraphicsSystem::m_initCameras( Engine& engine, World& world )
	{
		//init camera
		auto camera = Camera::EMPLACE_TO_WORLD( engine, world );
		if( boost::lexical_cast< bool >(*m_configValues.fullscreen) )
		{
			//fullscreen
			camera->getComponent<Camera>()->setRenderResolution(
				sf::Vector2u(
				boost::lexical_cast< int >(*m_configValues.render_resx),
				boost::lexical_cast< int >(*m_configValues.render_resy) ) );
		}
		else
		{
			//no fullscreen
			//ignores: render_resx, render_resy
		}
		m_cameraContainerMutex.lock();
		m_cameras.push_back( camera );
		m_cameraContainerMutex.unlock();
	}

	std::vector<std::shared_ptr<Entity>> GraphicsSystem::getCameras() const
	{
		m_cameraContainerMutex.lock();
		auto retVal = m_cameras;
		m_cameraContainerMutex.unlock();
		return retVal;
	}

	GraphicsSystem::~GraphicsSystem()
	{
		m_terminateDrawingThread();
	}

	void GraphicsSystem::m_terminateDrawingThread()
	{
		//safely terminate drawing thread
		m_drawingShouldTerminate = true;
		while( m_drawingIsActive )
			sleep( sf::milliseconds( 1 ) );
		sleep( sf::milliseconds( 1 ) );
	}

	void GraphicsSystem::m_launchDrawingThread( RenderWindow& renderWindow )
	{
		m_drawingShouldTerminate = false;
		m_drawingIsActive = true;

		//launch drawing thread
		thread drawingThread(
			drawingThreadFunction,
			ref( renderWindow ),
			ref( m_drawableEntityMutex ),
			ref( m_drawableEntities ),
			ref( m_cameraContainerMutex ),
			ref( m_cameras ),
			ref( m_drawingThreadFrameTime ),
			ref( m_drawingShouldTerminate ),
			ref( m_drawingIsActive )
			);
		drawingThread.detach();
	}

	void drawingThreadFunction( sf::RenderWindow& renderWindow,
								std::mutex& drawableEntitiesMutex,
								EntityManager::EntityContainer& drawableEntities,
								std::mutex& cameraContainerMutex,
								CameraContainer& cameraContainer,
								int& drawingThreadFrameTime,
								bool& shouldTerminate,
								bool& drawingIsActive )
	{
		renderWindow.setActive( true );
		sf::Clock thisFrameTime;

		while( !shouldTerminate )
		{
			drawableEntitiesMutex.lock();
			cameraContainerMutex.lock();

			drawingThreadFrameTime = thisFrameTime.restart().asMilliseconds();

			renderWindow.clear( Color::Red );

			//for every camera state information
			for( const auto& camera : cameraContainer )
				camera->getComponent<Camera>()->drawSpritesToRenderWindow( renderWindow, drawableEntities );

			drawableEntitiesMutex.unlock();
			cameraContainerMutex.unlock();

			renderWindow.display();
		}
		drawingIsActive = false;
		return;
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

	const size_t GraphicsSystem::type_hash = getRuntimeTypeInfo<GraphicsSystem>();

}