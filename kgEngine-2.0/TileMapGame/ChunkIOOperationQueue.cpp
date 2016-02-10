#include "ChunkIOOperationQueue.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	void ChunkIOOperationQueue::finishAllOperationsOnChunk( const Chunk& chunk )
	{
		vector<OperationQueue::iterator> toRemove;
		list<OperationQueue::iterator> operationsToFinish;

		for( auto it = m_operations.begin(); it != m_operations.end(); ++it )
			if( (*it)->getChunkToOperateOn() == chunk )
				operationsToFinish.push_back( it );

		for( auto it = operationsToFinish.begin(); it != operationsToFinish.end(); ++it )
		{
			(**it)->execute();
			toRemove.push_back( *it );
		}

		for( auto el : toRemove )
			m_operations.remove( *el );
	}

	void ChunkIOOperationQueue::setChunkIOCountPerFrame( int chunkIOCount )
	{
		m_ioCountPerFrame = chunkIOCount;
	}

	void ChunkIOOperationQueue::addOperation( std::unique_ptr<ChunkIOOperation>&& operation )
	{
		finishAllOperationsOnChunk( operation->getChunkToOperateOn() );
		m_operations.push_back( move( operation ) );
	}

	void ChunkIOOperationQueue::finishPreparedOperations()
	{
		unsigned int ioCount = 0;
		vector<OperationQueue::iterator> toRemove;

		for( auto it = m_operations.begin(); it != m_operations.end(); ++it )
		{
			if( ioCount > m_ioCountPerFrame )
				break;

			(*it)->execute();
			toRemove.push_back( it );
			ioCount++;
		}

		for( auto el : toRemove )
			m_operations.remove( *el );
	}

	void ChunkIOOperationQueue::completeAllOperations()
	{
		while( m_operations.size() != 0 )
			finishPreparedOperations();
	}
}