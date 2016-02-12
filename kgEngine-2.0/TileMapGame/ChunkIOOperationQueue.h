#pragma once
#include "stdafx.h"
#include "ChunkIOOperation.h"

namespace kg
{
	class ChunkIOOperationQueue
	{
		unsigned int m_ioCountPerFrame;

		typedef std::deque<std::unique_ptr<ChunkIOOperation>> OperationQueue;
		typedef std::list<std::unique_ptr<ChunkIOOperation>> OperationList;
		OperationQueue m_addedOperations;//added, but not running yet
		OperationList m_runningOperations;//running, not finished yet

		void startAllOperationsOnChunk( const Chunk& chunk );
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