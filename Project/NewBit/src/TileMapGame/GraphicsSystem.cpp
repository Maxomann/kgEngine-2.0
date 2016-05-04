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
		m_connectToSignal( world.s_entities_added, &GraphicsSystem::m_onEntityAddedToWorld );
		m_connectToSignal( world.s_entities_removed, &GraphicsSystem::m_onEntityRemovedFromWorld );

		//get config values
		m_configValues.antialiasing = configFile->getDataReplaceIfInvalid( ANTIALIASING, ANTIALIASING_DEFAULT );
		m_configValues.fullscreen = configFile->getDataReplaceIfInvalid( FULLSCREEN, FULLSCREEN_DEFAULT );
		m_configValues.window_resx = configFile->getDataReplaceIfInvalid( WINDOW_RESX, WINDOW_RESX_DEFAULT );
		m_configValues.window_resy = configFile->getDataReplaceIfInvalid( WINDOW_RESY, WINDOW_RESY_DEFAULT );
		m_configValues.vsync = configFile->getDataReplaceIfInvalid( VSYNC, VSYNC_DEFAULT );
		m_configValues.window_name = configFile->getDataReplaceIfInvalid( WINDOW_NAME, WINDOW_NAME_DEFAULT );
		m_configValues.drawDistance = configFile->getDataReplaceIfInvalid( DRAW_DISTANCE, DRAW_DISTANCE_DEFAULT );

		//init window
		sf::ContextSettings contextSettings;
		contextSettings.antialiasingLevel = m_configValues.antialiasing;

		if( m_configValues.fullscreen )
		{
			//fullscreen
			engine.renderWindow.create(
				sf::VideoMode::getDesktopMode(),//ignores: window_resx, window_resy
				m_configValues.window_name,
				sf::Style::Fullscreen,
				contextSettings );
		}
		else
		{
			//no fullscreen
			engine.renderWindow.create(
				sf::VideoMode(
					m_configValues.window_resx,
					m_configValues.window_resy, 32 ),
				m_configValues.window_name,
				sf::Style::Close,
				contextSettings );
		}

		glEnable( GL_SCISSOR_TEST );
		glScissor( 0, 0, engine.renderWindow.getSize().x, engine.renderWindow.getSize().y );
		engine.renderWindow.setVerticalSyncEnabled( m_configValues.vsync );
		engine.renderWindow.setActive( true );

		setDrawDistance( m_configValues.drawDistance );

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

		engine.renderWindow.setTitle( m_configValues.window_name +
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

	void GraphicsSystem::m_onEntityAddedToWorld( const EntityPointerContainer& entities )
	{
		m_toDrawContainer.addEntities_try( entities );
	}

	void GraphicsSystem::m_onEntityRemovedFromWorld( const EntityPointerContainer& entities )
	{
		EntityPointerContainer entitiesWithGraphics;
		EntityPointerContainer entitiesWithCamera;
		for( auto& el : entities )
		{
			if( el->hasComponent<Graphics>() )
				entitiesWithGraphics.push_back( el );
			if( el->hasComponent<Camera>() )
				entitiesWithCamera.push_back( el );
		}

		//remove entities
		m_toDrawContainer.removeEntities_try( entitiesWithGraphics );

		for( auto& entity : entitiesWithCamera )
		{
			auto it = remove( m_cameras.begin(), m_cameras.end(), entity );
			if( it != m_cameras.end() )
				m_cameras.erase( it );
		}
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

		// draw
		for( const auto& camera : m_cameras )
			camera->getComponent<Camera>()->drawSpritesToRenderWindow( *r_renderWindow, m_toDrawContainer );

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

	const int GraphicsSystem::DRAW_DISTANCE_DEFAULT = 3000;

	const bool GraphicsSystem::VSYNC_DEFAULT = true;

	const int GraphicsSystem::WINDOW_RESY_DEFAULT = 720;

	const int GraphicsSystem::WINDOW_RESX_DEFAULT = 1280;

	const bool GraphicsSystem::FULLSCREEN_DEFAULT = false;

	const int GraphicsSystem::ANTIALIASING_DEFAULT = 0;

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