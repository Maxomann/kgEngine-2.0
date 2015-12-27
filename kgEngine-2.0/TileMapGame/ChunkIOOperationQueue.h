#pragma once
#include "stdafx.h"
#include "ChunkIOOperation.h"

namespace kg
{
	class ChunkIOOperationQueue
	{
	public:
		void setChunkIOCountPerFrame( int chunkIOCount );

		// only one operation can be active for a specific chunk at once
		// calls ChunkIOOperation::execute_prepare()
		void addOperation( std::unique_ptr<ChunkIOOperation>&& operation );

		void finishPreparedOperations( World& world, std::vector<Chunk>& chunks );

		void finishAllOperations( World& world, std::vector<Chunk>& chunks );
	};
}