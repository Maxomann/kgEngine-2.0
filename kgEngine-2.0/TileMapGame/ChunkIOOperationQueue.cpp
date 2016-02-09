#include "ChunkIOOperationQueue.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	void ChunkIOOperationQueue::setChunkIOCountPerFrame( int chunkIOCount )
	{
		m_ioCountPerFrame = chunkIOCount;
	}

	void ChunkIOOperationQueue::addOperation( std::unique_ptr<ChunkIOOperation>&& operation )
	{
		operation->execute_prepare();
		operation->execute_main();
		m_operations.push_back( move( operation ) );
	}

	void ChunkIOOperationQueue::finishPreparedOperations()
	{
		unsigned int ioCount = 0;
		vector<OperationQueue::iterator> toRemove;

		for( auto it = m_operations.begin(); it != m_operations.end(); ++it )
			if( (*it)->isReadyToFinish() )
			{
				if( ioCount > m_ioCountPerFrame )
					break;

				(*it)->execute_finish();
				toRemove.push_back( it );
				ioCount++;
			}

		for( auto el : toRemove )
			m_operations.erase( remove( m_operations.begin(), m_operations.end(), *el ) );
	}

	void ChunkIOOperationQueue::completeAllOperations()
	{
		while( m_operations.size() != 0 )
			finishPreparedOperations();
	}
}