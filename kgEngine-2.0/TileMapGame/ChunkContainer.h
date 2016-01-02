#pragma once
#include "stdafx.h"
#include "Chunk.h"

namespace kg
{
	class ChunkContainer
	{
		std::vector<Chunk> m_chunks;

	public:
		Chunk& getChunk( const ChunkPosition& position );
		const Chunk& getChunk_const( const ChunkPosition& position )const;//will throw if chunk does not exist
		bool doesChunkExist( const ChunkPosition& position )const;

		std::vector<Chunk*> getAllLoadedChunks();

		void clear();
	};
}