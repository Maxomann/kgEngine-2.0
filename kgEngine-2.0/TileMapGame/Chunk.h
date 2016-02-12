#pragma once
#include "stdafx.h"
#include "Transformation.h"

namespace kg
{
	class Chunk
	{
	public:
		enum class State
		{
			UNINITIALIZED,
			LOADING,
			LOADED,
			ERR
		};

	private:

		const ChunkPosition m_position;
		World::EntityPointerContainer m_entities;

		// IO
		State m_state = State::UNINITIALIZED;

	public:
		Chunk( const ChunkPosition& position );

		const ChunkPosition& getPosition()const;

		void addEntity( Entity* entity );
		void removeEntity( Entity* entity );

		const World::EntityPointerContainer& getEntities()const;

		// IO
		State getState()const;
		void setState( State state );

		std::string getSavename()const;//return Chunk::getChunkSavename(thisPostion);
		static std::string getChunkSavename( const ChunkPosition& chunkPosition );

		bool operator==( const Chunk& rhs )const;

		// returns the position in chunks for the position of an entity
		static ChunkPosition calculateChunkPositionForPosition2d( const Position2d& position2d );
	};
}