#pragma once
#include "stdafx.h"
#include "ChunkLoadingOperation.h"

namespace kg
{
	class ChunkLoadingOperationQueue
	{
	public:
		void setChunkLoadCountPerFrame( int chunkLoadCount );

		void addOperation( ChunkLoadingOperation&& operation );
		void addOperation( ChunkUnloadingOperation&& operation );

		void update( World& world, std::vector<Chunk>& chunks );

		void cancelOrFinishAllOperations();
	};
}