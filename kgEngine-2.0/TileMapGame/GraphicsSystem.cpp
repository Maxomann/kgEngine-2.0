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

		sf::Clock c;

		//collect DrawingStateInformation
		DrawingStateInformation dsi;
		for( const auto& el : m_cameras )
			dsi.first.push_back( el->getComponent<Camera>()->getStateInformation() );
		for( const auto& el : world.getEntitiesThatHaveComponent<Graphics>() )
			dsi.second.push_back( el->getComponent<Graphics>()->getStateInformation() );
		m_drawingInformationContainer.push( dsi );

		engine.renderWindow.setTitle( m_configValues.window_name.toString() +
									  " " +
									  to_string( frameTime.asMilliseconds() ) +
									  " :: " +
									  to_string( m_drawingThreadFrameTime ) +
									  " ++ " +
									  to_string(c.restart().asMilliseconds()));

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

	void GraphicsSystem::m_onSavegameOpened( Engine& engine )
	{
		m_shouldInitCameras = true;
	}

	void GraphicsSystem::m_onSavegameClosed()
	{
		m_cameras.clear();
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
		m_cameras.push_back( camera );
	}

	std::vector<std::shared_ptr<Entity>>& GraphicsSystem::getCameras()
	{
		return m_cameras;
	}

	GraphicsSystem::~GraphicsSystem()
	{
		m_terminateDrawingThread();
	}

	void GraphicsSystem::m_terminateDrawingThread()
	{
		//safely terminate drawing thread
		m_drawingShouldTerminate = true;
		while( !m_drawingHasTerminated )
			sleep( sf::milliseconds( 1 ) );
		sleep( sf::milliseconds( 1 ) );
	}

	void GraphicsSystem::m_launchDrawingThread( RenderWindow& renderWindow )
	{
		m_drawingShouldTerminate = false;
		m_drawingHasTerminated = false;

		//launch drawing thread
		thread drawingThread(
			drawingThreadFunction,
			ref( renderWindow ),
			ref( m_drawingInformationContainer ),
			ref( m_drawingThreadFrameTime ),
			ref( m_drawingShouldTerminate ),
			ref( m_drawingHasTerminated )
			);
		drawingThread.detach();
	}

	void drawingThreadFunction( sf::RenderWindow& renderWindow,
								SwapContainer<DrawingStateInformation, std::stack<DrawingStateInformation>>& drawingInformationContainer,
								int& drawingThreadFrameTime,
								bool& shouldTerminate,
								bool& hasTerminated )
	{
		renderWindow.setActive( true );
		map<int, map<int, RenderTexture>> renderTexturesBySize;
		sf::Clock thisFrameTime;

		while( !shouldTerminate )
		{
			drawingThreadFrameTime = thisFrameTime.restart().asMilliseconds();

			auto container = drawingInformationContainer.getContent();
			if( container->size() > 0 )
			{
				renderWindow.clear( Color::Red );
				auto relevantInformation = container->top();

				//draw here
				/*OLD CODE FROM CAMERA
				m_texture.clear( Color::Green );
				m_texture.setView( m_view );

				auto toDraw = world.getEntitiesThatHaveComponent<Graphics>();
				map<int, map<int, map<int, std::vector<shared_ptr<Entity>>>>> toDrawSorted;//by Z value by Y value by X value;
				auto cameraRect = r_transformation->getGlobalBounds();
				for( auto& el : toDraw )
				{
				auto graphics = el->getComponent<Graphics>();
				auto toDrawTransformationComponent = el->getComponent<Transformation>();
				auto toDrawGlobalBounds = toDrawTransformationComponent->getGlobalBounds();

				if( toDrawGlobalBounds.intersects( cameraRect ) )//only add if visible on this camera
				{
				auto toDrawPosition = toDrawTransformationComponent->getPosition();
				auto zValue = toDrawTransformationComponent->getZValue();
				toDrawSorted[zValue][toDrawPosition.y][toDrawPosition.x].push_back( el );
				}
				}

				batch::SpriteBatch spriteBatch;
				spriteBatch.begin();

				for( const auto& Z : toDrawSorted )
				for( const auto& Y : Z.second )
				for( const auto& X : Y.second )
				for( const auto& entity : X.second )
				entity->getComponent<Graphics>()->drawToSpriteBatch( spriteBatch );

				spriteBatch.end();
				m_texture.draw( spriteBatch );

				m_texture.display();
				m_texture.setView( m_texture.getDefaultView() );
				*/


				//for every camera state information
				for( const auto& camera : relevantInformation.first )
				{
					//create render texture if needed (they will not be destroyed until funtion terminates)
					auto it = renderTexturesBySize.find( camera.renderResolution.x );
					if( it == end( renderTexturesBySize ) )
					{
						renderTexturesBySize[camera.renderResolution.x][camera.renderResolution.y].create( camera.renderResolution.x, camera.renderResolution.y );
					}
					else
					{
						auto it2 = it->second.find( camera.renderResolution.y );
						if( it2 == end( it->second ) )
						{
							renderTexturesBySize[camera.renderResolution.x][camera.renderResolution.y].create( camera.renderResolution.x, camera.renderResolution.y );
						}
					}


					auto& renderTexture = renderTexturesBySize[camera.renderResolution.x][camera.renderResolution.y];
					Sprite renderTextureSprite;
					map<int, map<int, map<int, std::vector<shared_ptr<Drawable>>>>> toDrawSorted;//Z Y X


					//sort toDraws
					for( const auto& toDraw : relevantInformation.second )
					{
						//if toDraw is seen on camera
						if( camera.globalBounds.intersects( toDraw.globalBounds ) )
						{
							//sort
							toDrawSorted
								[toDraw.zValue]//Z
							[toDraw.globalBounds.top + toDraw.globalBounds.height]//Y
							[toDraw.globalBounds.left]//X
							.push_back( toDraw.drawable );
						}
					}


					//draw drawables
					renderTexture.clear( Color::Green );
					renderTexture.setView( camera.view );
					for( const auto& Z : toDrawSorted )
						for( const auto& Y : Z.second )
							for( const auto& X : Y.second )
								for( const auto& toDraw : X.second )
									renderTexture.draw( *toDraw );
					renderTexture.display();
					renderTexture.setView( renderTexture.getDefaultView() );

					renderTextureSprite.setTexture( renderTexture.getTexture() );
					auto renderTextureSpriteBounds = renderTextureSprite.getGlobalBounds();
					renderTextureSprite.scale( camera.finalSize.x / renderTextureSpriteBounds.width,
											   camera.finalSize.y / renderTextureSpriteBounds.height );
					renderTextureSprite.setPosition( sf::Vector2f( camera.screenOffset ) );

					renderWindow.draw( renderTextureSprite );
				}

				renderWindow.display();
			}
			drawingInformationContainer.swap();
		}
		hasTerminated = true;
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
}
