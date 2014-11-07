#pragma once
#include "stdafx.h"
#include "Graphics.h"
#include "Camera.h"

namespace kg
{
	class GraphicsSystem : public System
	{
		std::shared_ptr<Entity> m_camera;

	public:
		virtual void init( Engine& engine, World& world );

		virtual void sfmlEvent( const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world );

		virtual double getUpdateImportance() const;

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;


		std::shared_ptr<Entity> getCamera();

		static const std::string PLUGIN_NAME;

	};
}
