#pragma once
#include "VBO.h"
#include "Position.h"

namespace kg
{
	class ChunkVBO : public SpriteVBO
	{
		PositionXY m_chunkPositionXY;

	public:
		ChunkVBO() = default;
		ChunkVBO( unsigned int vertexCapacity, GLenum usage );

		void setChunkPosition( const PositionXY& position );
		const PositionXY& getChunkPosition()const;

		//const bool operator==( const ChunkVBO& other )const;
	};
}