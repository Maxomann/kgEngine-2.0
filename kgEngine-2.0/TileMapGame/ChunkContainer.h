#pragma once
#include "stdafx.h"
#include "Chunk.h"

namespace kg
{
	class ChunkContainer
	{
		std::vector<Chunk> m_chunks;

	public:
		Chunk& getChunk( const Position2d& position );
	};
}