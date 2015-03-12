#pragma once
#include "stdafx.h"
#include "Transformation.h"

namespace kg
{
	class ChunkGenerator : public System
	{
	public:
		virtual void init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile ) override;

		virtual void sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent ) override;

		virtual void update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime ) override;

		virtual double getUpdateImportance() const override;

		virtual const std::string& getPluginName() const override;

		virtual Plugin::Id getPluginId() const override;

		void generateChunk( Engine& engine, World& world, const sf::Vector2i chunkPositionInChunks );

		static const std::string PLUGIN_NAME;

		static const size_t type_hash;
	};
}
