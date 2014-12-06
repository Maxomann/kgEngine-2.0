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
	};
}
