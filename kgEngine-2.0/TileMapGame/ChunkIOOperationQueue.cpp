#include "ChunkIOOperationQueue.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	void ChunkIOOperationQueue::abortAllOperationsOnChunk( const Chunk& chunk )
	{
		auto condition = [&]( const std::unique_ptr<ChunkIOOperation>& el )
		{
			return el->getChunkToOperateOn() == chunk;
		};

		for( auto it =
			 find_if( m_addedOperations.begin(), m_addedOperations.end(), condition );
			 it != m_addedOperations.end();
			 it = find_if( m_addedOperations.begin(), m_addedOperations.end(), condition ) )
		{
			(*it)->execute_abort();
			m_addedOperations.erase( remove( m_addedOperations.begin(), m_addedOperations.end(), *it ) );
		}

		for( auto it =
			 find_if( m_runningOperations.begin(), m_runningOperations.end(), condition );
			 it != m_runningOperations.end();
			 it = find_if( m_runningOperations.begin(), m_runningOperations.end(), condition ) )
		{
			(*it)->execute_abort();
			m_runningOperations.erase( remove( m_runningOperations.begin(), m_runningOperations.end(), *it ) );
		}
	}

	void ChunkIOOperationQueue::setChunkIOCountPerFrame( int chunkIOCount )
	{
		m_ioCountPerFrame = chunkIOCount;
	}

	void ChunkIOOperationQueue::addOperation( std::unique_ptr<ChunkIOOperation>&& operation )
	{
		abortAllOperationsOnChunk( operation->getChunkToOperateOn() );
		operation->execute_init();
		m_addedOperations.push_back( move( operation ) );
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
		for( unsigned int i = 0; i < m_ioCountPerFrame && m_addedOperations.size()>0; ++i )
		{
			auto& operation = m_addedOperations.front();
			operation->execute_main();
			m_runningOperations.push_back( move( operation ) );
			m_addedOperations.pop_front();
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