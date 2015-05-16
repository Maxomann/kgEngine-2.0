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

		m_drawableEntityMutex.lock();
		m_addedEntitiesCopy.insert( end( m_addedEntitiesCopy ), begin( m_addedEntities ), end( m_addedEntities ) );
		m_removedEntitiesCopy.insert( end( m_removedEntitiesCopy ), begin( m_removedEntities ), end( m_removedEntities ) );
		m_addedEntities.clear();
		m_removedEntities.clear();
		m_drawableEntityMutex.unlock();
		m_drawingThreadSyncMutex.lock();
		m_drawingThreadHasToWait = false;
		m_drawingThreadSyncMutex.unlock();

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
		m_cameraContainerMutexA.lock();
		auto retVal = m_cameras.at( index );
		m_cameraContainerMutexA.unlock();
		return retVal;
	}

	void GraphicsSystem::m_onEntityAddedToWorld( const std::shared_ptr<Entity>& entity )
	{
		if( entity->hasComponent<Graphics>() )
			m_addedEntities.push_back( entity );
		auto it = find( m_removedEntities.begin(), m_removedEntities.end(), entity );
		if( it != m_removedEntities.end() )
			m_removedEntities.erase( it );
	}

	void GraphicsSystem::m_onEntityRemovedFromWorld( const std::shared_ptr<Entity>& entity )
	{
		if( entity->hasComponent<Graphics>() )
			m_removedEntities.push_back( entity );
		auto it = find( m_addedEntities.begin(), m_addedEntities.end(), entity );
		if( it != m_addedEntities.end() )
			m_addedEntities.erase( it );
	}

	void GraphicsSystem::m_onSavegameOpened( Engine& engine )
	{
		m_shouldInitCameras = true;
	}

	void GraphicsSystem::m_onSavegameClosed()
	{
		m_cameraContainerMutexA.lock();
		m_cameraContainerMutexB.lock();
		m_cameras.clear();
		m_cameraContainerMutexB.unlock();
		m_cameraContainerMutexA.unlock();
	}

	void GraphicsSystem::m_initCameras( Engine& engine, World& world )
	{
		//init camera
		auto camera = Camera::EMPLACE_TO_WORLD( engine, world );
		m_cameraContainerMutexA.lock();
		m_cameraContainerMutexB.lock();
		m_cameras.push_back( camera );
		m_cameraContainerMutexB.unlock();
		m_cameraContainerMutexA.unlock();
	}

	kg::CameraContainer GraphicsSystem::getCameras() const
	{
		m_cameraContainerMutexA.lock();
		auto retVal = m_cameras;
		m_cameraContainerMutexA.unlock();
		return retVal;
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
		m_drawingThreadSyncMutex.lock();
		m_drawingThreadHasToWait = false;
		m_drawingThreadSyncMutex.unlock();
		while( m_drawingIsActive )
			sleep( sf::milliseconds( 1 ) );
		sleep( sf::milliseconds( 1 ) );
	}

	void GraphicsSystem::m_launchDrawingThread( sf::RenderWindow& renderWindow )
	{
		m_drawingShouldTerminate = false;
		m_drawingIsActive = true;

		//launch drawing thread
		thread drawingThread(
			drawingThreadFunction,
			std::ref( renderWindow ),
			std::ref( m_drawableEntityMutex ),
			std::ref( m_cameraContainerMutexB ),
			std::ref( m_cameras ),
			std::ref( m_addedEntitiesCopy ),
			std::ref( m_removedEntitiesCopy ),
			std::ref( m_drawingThreadFrameTime ),
			std::ref( m_drawingShouldTerminate ),
			std::ref( m_drawingIsActive ),
			std::ref( m_drawingThreadSyncMutex ),
			std::ref( m_drawingThreadHasToWait )
			);
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

	void drawingThreadFunction( sf::RenderWindow& renderWindow,
								boost::mutex& m_drawableEntityMutex,
								boost::mutex& cameraContainerMutex,
								CameraContainer& cameraContainer,
								EntityManager::EntityContainer& addedEntitiesCopy,
								EntityManager::EntityContainer& removedEntitiesCopy,
								int& drawingThreadFrameTime,
								bool& shouldTerminate,
								bool& drawingIsActive,
								boost::mutex& syncMutex,
								bool& threadHasToWait )
	{
		renderWindow.setActive( true );
		sf::Clock thisFrameTime;

		vector<tuple<Vector3i, std::shared_ptr<Entity>, Graphics*>> toDrawSorted;

		while( !shouldTerminate )
		{
			while( true )
			{
				//lock
				syncMutex.lock();
				if( !threadHasToWait )
				{
					threadHasToWait = true;
					break;
				}
				else
					syncMutex.unlock();
			}
			syncMutex.unlock();


			drawingThreadFrameTime = thisFrameTime.restart().asMilliseconds();

			renderWindow.clear( Color::Green );

			//LOCK
			cameraContainerMutex.lock();
			m_drawableEntityMutex.lock();

			//remove
			for( const auto& el : removedEntitiesCopy )
			{
				auto it = findInToDraw( toDrawSorted, el );
				if( it != toDrawSorted.end() )
					toDrawSorted.erase( findInToDraw( toDrawSorted, el ) );
			}

			//add
			bool needsSort = (addedEntitiesCopy.size() != 0);

			for( auto& el : addedEntitiesCopy )
			{
				const auto transformationComponent = el->getComponent<Transformation>();
				toDrawSorted.push_back( make_tuple( transformationComponent->getXYZValues(), el, el->getComponent<Graphics>() ) );
			}

			addedEntitiesCopy.clear();
			removedEntitiesCopy.clear();

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
			for( const auto& camera : cameraContainer )
				camera->getComponent<Camera>()->drawSpritesToRenderWindow( renderWindow, toDrawSorted );

			m_drawableEntityMutex.unlock();
			cameraContainerMutex.unlock();
			//UNLOCK

			renderWindow.display();
		}
		drawingIsActive = false;
		return;
	}

	const std::string GraphicsSystem::WINDOW_NAME_DEFAULT = "DefaultWindowName";

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

	const std::string GraphicsSystem::PLUGIN_NAME = "GraphicsSystem";

	const size_t GraphicsSystem::type_hash = getRuntimeTypeInfo<GraphicsSystem>();

}
