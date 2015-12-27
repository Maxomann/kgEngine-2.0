#include "ChunkContainer.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	Chunk& ChunkContainer::getChunk( const Position2d& position )
	{
		auto it = find_if( begin( m_chunks ), end( m_chunks ), [&]( const Chunk& chunk )
		{
			chunk.getPosition() == position;
		} );

		if( it != end( m_chunks ) )
			return *it;
		else
		{
			m_chunks.push_back( Chunk( position ) );
			return m_chunks.back();
		}
	}
}