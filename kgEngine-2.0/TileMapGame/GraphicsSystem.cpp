#include "GraphicsSystem.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GraphicsSystem::init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile )
	{
		r_renderWindow = &engine.renderWindow;
		r_gui = &engine.inputManager.gui;

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
		m_configValues.vsync = &configFile->getData( VSYNC );
		m_configValues.window_name = &configFile->getData( WINDOW_NAME );
		m_configValues.drawDistance = &configFile->getData( DRAW_DISTANCE );

		//set them if invalid ( and retrieve them a second time )
		if( !m_configValues.antialiasing->size() )
			*m_configValues.antialiasing = ANTIALIASING_DEFAULT;
		if( !m_configValues.fullscreen->size() )
			*m_configValues.fullscreen = FULLSCREEN_DEFAULT;
		if( !m_configValues.window_resx->size() )
			*m_configValues.window_resx = WINDOW_RESX_DEFAULT;
		if( !m_configValues.window_resy->size() )
			*m_configValues.window_resy = WINDOW_RESY_DEFAULT;
		if( !m_configValues.vsync->size() )
			*m_configValues.vsync = VSYNC_DEFAULT;
		if( !m_configValues.window_name->size() )
			*m_configValues.window_name = WINDOW_NAME_DEFAULT;
		if( !m_configValues.drawDistance->size() )
			*m_configValues.drawDistance = DRAW_DISTANCE_DEFAULT;

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

		setDrawDistance( boost::lexical_cast< unsigned int >(*m_configValues.drawDistance) );

		engine.inputManager.gui.setWindow( engine.renderWindow );
		engine.inputManager.gui.setGlobalFont( engine.resourceManager.getResource<sf::Font>(
			Constants::PACKAGE_NAME,
			Folder::FONTS + "DejaVuSans.ttf"
			) );

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

		m_drawableEntityMutex.lock();
		m_addedEntitiesCopy.insert( end( m_addedEntitiesCopy ), begin( m_addedEntities ), end( m_addedEntities ) );
		m_removedEntitiesCopy.insert( end( m_removedEntitiesCopy ), begin( m_removedEntities ), end( m_removedEntities ) );
		m_addedEntities.clear();
		m_removedEntities.clear();
		m_drawableEntityMutex.unlock();
		m_syncMutex.lock();
		m_threadHasToWait = false;
		m_syncMutex.unlock();

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

	void GraphicsSystem::m_onEntityAddedToWorld( const std::shared_ptr<Entity>& entity )
	{
		if( entity->hasComponent<Graphics>() )
			m_addedEntities.push_back( entity );
	}

	void GraphicsSystem::m_onEntityRemovedFromWorld( const std::shared_ptr<Entity>& entity )
	{
		if( entity->hasComponent<Graphics>() )
			m_removedEntities.push_back( entity );
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

	void GraphicsSystem::m_initCameras( Engine& engine, World& world )
	{
		//init camera
		auto camera = Camera::EMPLACE_TO_WORLD( engine, world, m_drawDistanceMutex, &m_drawDistance );
		m_cameraContainerMutex.lock();
		m_cameras.push_back( camera );
		m_cameraContainerMutex.unlock();
	}

	kg::CameraContainer GraphicsSystem::getCameras() const
	{
		m_cameraContainerMutex.lock();
		auto retVal = m_cameras;
		m_cameraContainerMutex.unlock();
		return retVal;
	}

	void GraphicsSystem::setDrawDistance( const unsigned int& drawDistance )
	{
		m_drawDistanceMutex.lock();
		m_drawDistance = drawDistance;
		m_drawDistanceMutex.unlock();
	}

	unsigned int GraphicsSystem::getDrawDistance() const
	{
		m_drawDistanceMutex.lock();
		auto copy = m_drawDistance;
		m_drawDistanceMutex.unlock();
		return copy;
	}

	const size_t& GraphicsSystem::getRTTI_hash() const
	{
		return type_hash;
	}

	GraphicsSystem::GraphicsSystem()
	{
		glewInit();
	}

	GraphicsSystem::~GraphicsSystem()
	{
		m_terminateDrawingThread();
	}

	void GraphicsSystem::m_terminateDrawingThread()
	{
		//safely terminate drawing thread
		m_drawingShouldTerminate = true;
		m_syncMutex.lock();
		m_threadHasToWait = false;
		m_syncMutex.unlock();
		while( m_drawingIsActive )
			sleep( sf::milliseconds( 1 ) );
		sleep( sf::milliseconds( 1 ) );
	}

	void GraphicsSystem::m_launchDrawingThread( sf::RenderWindow& renderWindow )
	{
		m_drawingShouldTerminate = false;

		//launch drawing thread
		thread drawingThread( &GraphicsSystem::drawingThreadFunction, this );
		drawingThread.detach();
	}

	vector<tuple<Vector3i, std::shared_ptr<Entity>, Graphics*>>::iterator findInToDraw( vector<tuple<Vector3i, std::shared_ptr<Entity>, Graphics*>>& container,
																						std::shared_ptr<Entity> el )
	{
		for( auto it = container.begin(); it != container.end(); ++it )
			if( get<1>( *it ) == el )
				return it;
		return container.end();
	};

	void GraphicsSystem::drawingThreadFunction()
	{
		glEnable( GL_SCISSOR_TEST );
		glScissor( 0, 0, r_renderWindow->getSize().x, r_renderWindow->getSize().y );

		m_drawingIsActive = true;
		r_renderWindow->setActive( true );
		sf::Clock frameTimeClock;

		vector<tuple<Vector3i, std::shared_ptr<Entity>, Graphics*>> toDrawSorted;

		while( !m_drawingShouldTerminate )
		{
			while( true )
			{
				//lock
				m_syncMutex.lock();
				if( !m_threadHasToWait )
				{
					m_threadHasToWait = true;
					break;
				}
				else
					m_syncMutex.unlock();
			}


			frameTimeClock.restart().asMilliseconds();

			r_renderWindow->clear( Color::Green );

			//LOCK
			m_cameraContainerMutex.lock();
			m_drawableEntityMutex.lock();

			//remove
			toDrawSorted.erase( std::remove_if( toDrawSorted.begin(), toDrawSorted.end(), [&]( const tuple<Vector3i, std::shared_ptr<Entity>, Graphics*>& conel )
			{
				for( const auto& el : m_removedEntitiesCopy )
					if( get<1>( conel ) == el )
					{
						m_removedEntitiesCopy.erase( remove( m_removedEntitiesCopy.begin(), m_removedEntitiesCopy.end(), el ), m_removedEntitiesCopy.end() );
						return true;
					}
				return false;
			} ), toDrawSorted.end() );

			//add
			bool needsSort = (m_addedEntitiesCopy.size() != 0);

			for( auto& el : m_addedEntitiesCopy )
			{
				const auto transformationComponent = el->getComponent<Transformation>();
				toDrawSorted.push_back( make_tuple( transformationComponent->getXYZValues(), el, el->getComponent<Graphics>() ) );
			}

			m_addedEntitiesCopy.clear();
			m_removedEntitiesCopy.clear();

			//sort
			if( needsSort )
			{
				sort( begin( toDrawSorted ), end( toDrawSorted ), [](
					const tuple<Vector3i, std::shared_ptr<Entity>, Graphics*>& lhs,
					const tuple<Vector3i, std::shared_ptr<Entity>, Graphics*>& rhs )
				{
					const auto& vecl = get<0>( lhs );
					const auto& vecr = get<0>( rhs );

					if( vecr.z > vecl.z )
						return true;
					else if( vecr.z == vecl.z && vecr.y > vecl.y )
						return true;
					else if( vecr.y == vecl.y && vecr.x > vecl.x )
						return true;

					return false;
				} );
			}


			//draw
			for( const auto& camera : m_cameras )
				camera->getComponent<Camera>()->drawSpritesToRenderWindow( *r_renderWindow, toDrawSorted );

			m_drawableEntityMutex.unlock();
			m_cameraContainerMutex.unlock();
			//UNLOCK

			r_gui->draw();
			r_renderWindow->display();

			m_drawingThreadFrameTime = frameTimeClock.getElapsedTime().asMilliseconds();
			m_syncMutex.unlock();

		}
		m_drawingIsActive = false;
		return;
	}

	const std::string GraphicsSystem::WINDOW_NAME_DEFAULT = "DefaultWindowName";

	const std::string GraphicsSystem::DRAW_DISTANCE_DEFAULT = "500";

	const std::string GraphicsSystem::VSYNC_DEFAULT = "1";

	const std::string GraphicsSystem::WINDOW_RESY_DEFAULT = "720";

	const std::string GraphicsSystem::WINDOW_RESX_DEFAULT = "1280";

	const std::string GraphicsSystem::FULLSCREEN_DEFAULT = "0";

	const std::string GraphicsSystem::ANTIALIASING_DEFAULT = "0";

	const std::string GraphicsSystem::ANTIALIASING = "iAntialiasing";

	const std::string GraphicsSystem::VSYNC = "bVsync";

	const std::string GraphicsSystem::WINDOW_RESY = "iWindow_resy";

	const std::string GraphicsSystem::WINDOW_RESX = "iWindow_resx";

	const std::string GraphicsSystem::FULLSCREEN = "bFullscreen";

	const std::string GraphicsSystem::WINDOW_NAME = "sWindow_name";

	const std::string GraphicsSystem::DRAW_DISTANCE = "idrawDistance";

	const std::string GraphicsSystem::PLUGIN_NAME = "GraphicsSystem";

	const size_t GraphicsSystem::type_hash = getRuntimeTypeInfo<GraphicsSystem>();

}
