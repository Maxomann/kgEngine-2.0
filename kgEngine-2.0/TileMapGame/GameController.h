#pragma once
#include "stdafx.h"
#include "Camera.h"
#include "ChunkSystem.h"
#include "GraphicsSystem.h"

namespace kg
{
	class GameController : public System
	{
		GraphicsSystem* r_graphicsSystem;

		std::shared_ptr<Entity> m_test;

	public:
		virtual void init( Engine& engine, World& world, std::shared_ptr<ConfigFile>& configFile );

		virtual void sfmlEvent( Engine& engine, const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world, const sf::Time& frameTime );

		virtual double getUpdateImportance() const;

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;

		static const std::string PLUGIN_NAME;
	};
}
