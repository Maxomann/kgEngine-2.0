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
		m_connectToSignal( world.s_removeEntitiesOnRemoveList, &GraphicsSystem::m_onRemoveEntitiesFromRemoveList );

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

		glEnable( GL_SCISSOR_TEST );
		glScissor( 0, 0, engine.renderWindow.getSize().x, engine.renderWindow.getSize().y );
		engine.renderWindow.setVerticalSyncEnabled( boost::lexical_cast< bool >(*m_configValues.vsync) );
		engine.renderWindow.setActive( true );

		setDrawDistance( boost::lexical_cast< unsigned int >(*m_configValues.drawDistance) );

		engine.inputManager.gui.setWindow( engine.renderWindow );
		engine.inputManager.gui.setFont( engine.resourceManager.getResource<sf::Font>(
			Constants::PACKAGE_NAME,
			Folder::FONTS + "DejaVuSans.ttf"
			) );
	}

	void GraphicsSystem::sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent )
	{
		return;
	}

	void GraphicsSystem::update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime )
	{
		r_renderWindow->setActive( true );

		engine.renderWindow.setTitle( *m_configValues.window_name +
									  " " +
									  to_string( frameTime.asMilliseconds() ) +
									  " | EntityCount: " +
									  to_string( world.getEntityCount() ) );

		drawFunction();

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

	Entity* GraphicsSystem::getCamera( int index )
	{
		auto retVal = m_cameras.at( index );
		return retVal;
	}

	void GraphicsSystem::m_onEntityAddedToWorld( Entity* entity )
	{
		if( entity->hasComponent<Graphics>() )
		{
			const auto transformationComponent = entity->getComponent<Transformation>();
			m_toDrawSorted.push_back( make_tuple( transformationComponent->getXYZValues(),
												  entity,
												  entity->getComponent<Graphics>() ) );
		}
	}

	void GraphicsSystem::m_onEntityRemovedFromWorld( Entity* entity )
	{
		if( entity->hasComponent<Graphics>() || entity->hasComponent<Camera>() )
			m_removedEntities.push_back( entity );
	}

	void GraphicsSystem::m_onRemoveEntitiesFromRemoveList()
	{
		if( m_removedEntities.size() == 0 )
			return;

		//remove cameras
		m_cameras.erase( std::remove_if( m_cameras.begin(), m_cameras.end(), [&]( const Entity* conel )
		{
			for( const auto& el : m_removedEntities )
				if( conel == el )
					return true;
			return false;
		} ), m_cameras.end() );

		//remove
		m_toDrawSorted.erase( std::remove_if( m_toDrawSorted.begin(), m_toDrawSorted.end(), [&]( const tuple<Vector3i, Entity*, Graphics*>& conel )
		{
			for( const auto& el : m_removedEntities )
				if( get<1>( conel ) == el )
				{
					m_removedEntities.erase( remove( m_removedEntities.begin(), m_removedEntities.end(), el ), m_removedEntities.end() );
					return true;
				}
			return false;
		} ), m_toDrawSorted.end() );

		m_removedEntities.clear();
	}

	void GraphicsSystem::m_onSavegameOpened( Engine& engine, World& world )
	{
		m_initCameras( engine, world );
	}

	void GraphicsSystem::m_onSavegameClosed()
	{
		m_cameras.clear();
	}

	void GraphicsSystem::m_initCameras( Engine& engine, World& world )
	{
		//init camera
		auto* camera = world.addEntity( Camera::CREATE( engine, world, m_drawDistance ) );
		m_cameras.push_back( camera );
	}

	EntityManager::EntityPointerContainer GraphicsSystem::getCameras() const
	{
		return m_cameras;
	}

	void GraphicsSystem::setDrawDistance( const unsigned int& drawDistance )
	{
		m_drawDistance = drawDistance;
		for( auto& cam : m_cameras )
			cam->getComponent<Camera>()->setDrawDistance( m_drawDistance );
	}

	unsigned int GraphicsSystem::getDrawDistance() const
	{
		return m_drawDistance;
	}

	void GraphicsSystem::saveChangesToConfigFile( std::shared_ptr<ConfigFile>& configFile )
	{ }

	const size_t& GraphicsSystem::getRTTI_hash() const
	{
		return type_hash;
	}

	GraphicsSystem::GraphicsSystem()
	{
		glewInit();
	}

	vector<tuple<Vector3i, Entity*, Graphics*>>::iterator findInToDraw( vector<tuple<Vector3i, Entity*, Graphics*>>& container,
																		Entity* el )
	{
		for( auto it = container.begin(); it != container.end(); ++it )
			if( get<1>( *it ) == el )
				return it;
		return container.end();
	};

	void GraphicsSystem::drawFunction()
	{
		r_renderWindow->clear( Color::Green );

		// sort
		sort( begin( m_toDrawSorted ), end( m_toDrawSorted ), [](
			const tuple<Vector3i, Entity*, Graphics*>& lhs,
			const tuple<Vector3i, Entity*, Graphics*>& rhs )
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

		// draw
		for( const auto& camera : m_cameras )
			camera->getComponent<Camera>()->drawSpritesToRenderWindow( *r_renderWindow, m_toDrawSorted );

		r_gui->draw();
		r_renderWindow->display();

		//output GL_ERROR
		while( true )
		{
			GLenum err = glGetError();
			if( err == GL_NO_ERROR )
				break;
			cout << err << endl;
		}
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