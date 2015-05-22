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

		float m_cameraZoomFactor = 1;
		std::weak_ptr<Entity> m_camera;

		void saveOpenSavegame( Engine& engine, World& world, SaveManager& saveManager );
		void shutDown( Engine& engine, World& world, SaveManager& saveManager );

	public:
		virtual void init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile );

		virtual void sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime );

		virtual double getUpdateImportance() const;

		virtual const Plugin::Name& getPluginName() const;

		virtual Plugin::Id getPluginId()const;

		virtual const size_t& getRTTI_hash() const override;

		static const std::string PLUGIN_NAME;

		static const size_t type_hash;
	};
}
