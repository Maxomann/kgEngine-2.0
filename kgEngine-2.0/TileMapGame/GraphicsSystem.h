#pragma once
#include "stdafx.h"
#include "id.h"
#include "Camera.h"

namespace kg
{
	class GraphicsSystem : public System
	{
		std::vector<std::shared_ptr<Entity>> m_cameras;

		std::shared_ptr<ConfigFile> m_configFile;

	public:
		virtual void init( Engine& engine, World& world, std::shared_ptr<ConfigFile>& configFile );

		virtual void sfmlEvent( Engine& engine, const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world );

		virtual double getUpdateImportance() const;

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;


		std::shared_ptr<Entity>& getCamera( int index );

		/*void setWindowSize( const sf::Vector2i& size );
		void setWindowTitle( const std::string& title );*/


		static const std::string PLUGIN_NAME;


		//configuration values:
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
