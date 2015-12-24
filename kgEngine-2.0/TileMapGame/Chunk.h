#pragma once
#include "stdafx.h"

namespace kg
{
	struct ChunkPosition
	{
		int x;
		int y;
		int worldLayer;
	};

	class Chunk
	{
		const ChunkPosition m_position;
		World::EntityPointerContainer m_entities;

		// IO
		bool m_isLoaded = false;

		bool canBeLoadedFromFile()const;
		bool loadFromFile( SaveManager& saveManager )const;
		void saveToFile( SaveManager& saveManager )const;

	public:
		Chunk( const ChunkPosition& position );

		const ChunkPosition& getPosition()const;

		void addEntity( const Entity*& entity );
		const World::EntityPointerContainer& getEntities()const;

		// IO
		bool isLoaded()const;
		void ensureIsLoaded();
		void ensureIsUnloaded();
	};
}