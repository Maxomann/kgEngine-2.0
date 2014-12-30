#pragma once
#include "stdafx.h"
#include "Camera.h"
#include "ChunkSystem.h"
#include "GraphicsSystem.h"
#include "SaveSystem.h"

namespace kg
{
	class GameController : public System
	{
		GraphicsSystem* r_graphicsSystem;
		SavegameSystem* r_saveSystem;

	public:
		virtual void init( Engine& engine, World& world, std::shared_ptr<ConfigFile>& configFile );

		virtual void sfmlEvent( Engine& engine, const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime );

		virtual double getUpdateImportance() const;

		virtual const std::string& getPluginName() const;

		virtual Plugin::Id getPluginId()const;

		virtual void writeSaveInformation( SystemSaveInformation& writeTo ) override;

		virtual void loadSaveInformation( const SystemSaveInformation& loadFrom ) override;

		static const std::string PLUGIN_NAME;
	};
}
