#include "ChunkIOOperationQueue.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	void ChunkIOOperationQueue::finishAllOperationsOnChunk( const Chunk& chunk )
	{
		vector<OperationList::iterator> toRemove;
		list<OperationList::iterator> operationsToFinish;

		for( auto it = m_runningOperations.begin(); it != m_runningOperations.end(); ++it )
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
		operation->execute_init();
		m_addedOperations.push( move( operation ) );
	}

	void ChunkIOOperationQueue::finishPreparedOperations()
	{
		unsigned int ioCount = 0;
		vector<OperationList::iterator> toRemove;

		for( auto it = m_runningOperations.begin(); it != m_runningOperations.end(); ++it )
		{
			if( ioCount > m_ioCountPerFrame )
				break;

			if( (*it)->execute_finish_try() )
			{
				toRemove.push_back( it );
				ioCount++;
			}
		}

		for( auto el : toRemove )
			m_runningOperations.remove( *el );
	}

	void ChunkIOOperationQueue::startAddedOperations()
	{
		for( int i = 0; i < m_ioCountPerFrame; ++i )
		{
			auto& operation = m_addedOperations.front();
			operation->execute_main();
			m_runningOperations.push_back( move( operation ) );
			m_addedOperations.pop();
		}
	}

	void ChunkIOOperationQueue::update()
	{
		finishPreparedOperations();
		startAddedOperations();
	}

	void ChunkIOOperationQueue::completeAllOperations()
	{
		while( m_addedOperations.size() != 0 || m_runningOperations.size() != 0 )
		{
			startAddedOperations();
			finishPreparedOperations();
		}
	}
}