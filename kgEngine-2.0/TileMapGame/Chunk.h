#pragma once
#include "stdafx.h"
#include "Position.h"

namespace kg
{
	class Chunk
	{
		const Position2d m_position;
		World::EntityPointerContainer m_entities;

		// IO
		bool m_isLoaded = false;

	public:
		Chunk( const Position2d& position );

		const Position2d& getPosition()const;

		void addEntity( const Entity*& entity );
		const World::EntityPointerContainer& getEntities()const;

		// IO
		bool isLoaded()const;
		void setLoadState( bool isLoaded );

		std::string getSavename()const;//return Chunk::getChunkSavename(thisPostion);
		static std::string getChunkSavename( const Position2d& chunkPosition );
	};
}