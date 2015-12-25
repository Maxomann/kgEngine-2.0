#pragma once
#include "stdafx.h"

namespace kg
{
	class WorldGenerator : public Plugin
	{
	public:
		virtual void generateChunk( Engine& engine, World& world, const sf::Vector2i chunkPositionInChunks ) = 0;

		virtual const Plugin::Name& getPluginName() const override = 0;

		virtual Plugin::Id getPluginId() const override = 0;
	};
}