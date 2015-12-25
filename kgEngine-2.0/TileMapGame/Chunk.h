#pragma once
#include "stdafx.h"
#include "ChunkPosition.h"

namespace kg
{
	class Chunk
	{
		const ChunkPosition m_position;
		World::EntityPointerContainer m_entities;

		// IO
		bool m_isLoaded = false;

	public:
		Chunk( const ChunkPosition& position );

		const ChunkPosition& getPosition()const;

		void addEntity( const Entity*& entity );
		const World::EntityPointerContainer& getEntities()const;

		// IO
		bool isLoaded()const;
		void setLoadState( bool isLoaded );

		std::string getSavename()const;//return getChunkSavename(thisPostion);
		static std::string getChunkSavename( const ChunkPosition& chunkPosition );
	};
}