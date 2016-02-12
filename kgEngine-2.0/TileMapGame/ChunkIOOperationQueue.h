#pragma once
#include "stdafx.h"
#include "ChunkIOOperation.h"

namespace kg
{
	class ChunkIOOperationQueue
	{
		unsigned int m_ioCountPerFrame;

		typedef std::queue<std::unique_ptr<ChunkIOOperation>> OperationQueue;
		typedef std::list<std::unique_ptr<ChunkIOOperation>> OperationList;
		std::map<ChunkPosition, OperationQueue> m_addedOperations;//not running yet
		OperationList m_runningOperations;//not finished yet

		void finishAllOperationsOnChunk( const Chunk& chunk );

		// call this in ChunkSystem::update()
		// calls ChunkIOOperation::execute_finish()
		void finishPreparedOperations();

		void startAddedOperations();

	public:
		void setChunkIOCountPerFrame( int chunkIOCount );

		// only one operation can be active for a specific chunk at once
		// calls ChunkIOOperation::execute_prepare()
		// calls ChunkIOOperation::execute_main()
		void addOperation( std::unique_ptr<ChunkIOOperation>&& operation );

		void update();

		// may take a long time since this function may wait for many threads to finish
		void completeAllOperations();
	};
}