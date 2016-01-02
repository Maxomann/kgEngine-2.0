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

		// call this in ChunkSystem::update()
		//  calls ChunkIOOperation::execute_finish()
		void finishPreparedOperations();

		// may take a long time since this function may wait for many threads to finish
		void completeAllOperations();
	};
}