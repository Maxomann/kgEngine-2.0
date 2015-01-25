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

		void shutDown( Engine& engine, World& world, SaveManager& saveManager );

	public:
		virtual void init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile );

		virtual void sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime );

		virtual double getUpdateImportance() const;

		virtual const std::string& getPluginName() const;

		virtual Plugin::Id getPluginId()const;


		static const std::string PLUGIN_NAME;
	};
}
