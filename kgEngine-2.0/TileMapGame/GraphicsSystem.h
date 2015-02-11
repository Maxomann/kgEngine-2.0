#pragma once
#include "stdafx.h"
#include "id.h"
#include "Camera.h"

namespace kg
{
	//cameraStates, graphicsInformations
	typedef std::pair<std::vector<CameraStateInformation>, std::vector<GraphicsStateInformation>> DrawingStateInformation;

	void drawingThreadFunction( sf::RenderWindow& renderWindow,
								SwapContainer<DrawingStateInformation, std::stack<DrawingStateInformation>>& drawingInformationContainer,
								int& drawingThreadFrameTime,
								bool& shouldTerminate,
								bool& hasTerminated );


	class GraphicsSystem : public System, public CallbackReciever
	{
		std::shared_ptr<ConfigFile> m_configFile;
		struct ConfigValues
		{
			blueprint::Value antialiasing;
			blueprint::Value fullscreen;
			blueprint::Value window_resx;
			blueprint::Value window_resy;
			blueprint::Value render_resx;
			blueprint::Value render_resy;
			blueprint::Value vsync;
			blueprint::Value window_name;
		}m_configValues;


		std::vector<std::shared_ptr<Entity>> m_cameras;

		bool m_shouldInitCameras = true;

		void m_onSavegameOpened( Engine& engine );
		void m_onSavegameClosed();

		void m_initCameras( Engine& engine, World& world );


		void m_launchDrawingThread( sf::RenderWindow& renderWindow );
		void m_terminateDrawingThread();
		SwapContainer<DrawingStateInformation, std::stack<DrawingStateInformation>> m_drawingInformationContainer;
		bool m_drawingShouldTerminate = false;
		bool m_drawingHasTerminated = false;
		int m_drawingThreadFrameTime = -1;

	public:
		~GraphicsSystem();

		virtual void init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile );

		virtual void sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime );

		virtual double getUpdateImportance() const;

		virtual const std::string& getPluginName() const;

		virtual Plugin::Id getPluginId()const;



		std::shared_ptr<Entity>& getCamera( int index );
		std::vector<std::shared_ptr<Entity>>& getCameras();

		/*void setWindowSize( const sf::Vector2i& size );
		void setWindowTitle( const std::string& title );*/

		static const std::string PLUGIN_NAME;

		//configuration default values:
		static const std::string ANTIALIASING;//
		static const std::string FULLSCREEN;//
		static const std::string WINDOW_RESX;//only in windowed mode
		static const std::string WINDOW_RESY;//only in windowed mode
		static const std::string RENDER_RESX;//only in fullscreen mode (otherwise set to window size)
		static const std::string RENDER_RESY;//only in fullscreen mode (otherwise set to window size)
		static const std::string VSYNC;//
		static const std::string WINDOW_NAME;//

		static const std::string ANTIALIASING_DEFAULT;
		static const std::string FULLSCREEN_DEFAULT;
		static const std::string WINDOW_RESX_DEFAULT;
		static const std::string WINDOW_RESY_DEFAULT;
		static const std::string RENDER_RESX_DEFAULT;
		static const std::string RENDER_RESY_DEFAULT;
		static const std::string VSYNC_DEFAULT;
		static const std::string WINDOW_NAME_DEFAULT;
	};
}
