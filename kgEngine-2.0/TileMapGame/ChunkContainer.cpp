#include "ChunkContainer.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	Chunk& ChunkContainer::getChunk( const ChunkPosition& position )
	{
		auto it = find_if( begin( m_chunks ), end( m_chunks ), [&]( const Chunk& chunk )
		{
			return chunk.getPosition() == position;
		} );

		if( it != end( m_chunks ) )
			return *it;
		else
		{
			m_chunks.push_back( Chunk( position ) );
			return m_chunks.back();
		}
	}

	const Chunk& ChunkContainer::getChunk_const( const ChunkPosition& position )const
	{
		auto it = find_if( begin( m_chunks ), end( m_chunks ), [&]( const Chunk& chunk )
		{
			return chunk.getPosition() == position;
		} );

		if( it != end( m_chunks ) )
			return *it;
		else
			throw exception();
	}

	bool ChunkContainer::doesChunkExist( const ChunkPosition& position ) const
	{
		auto it = find_if( begin( m_chunks ), end( m_chunks ), [&]( const Chunk& chunk )
		{
			return chunk.getPosition() == position;
		} );

		return (it != end( m_chunks ));
	}

	std::vector<Chunk*> ChunkContainer::getAllLoadedChunks()
	{
		std::vector<Chunk*> retVal;

		for( auto& el : m_chunks )
			if( el.isLoaded() )
				retVal.push_back( &el );

		return retVal;
	}

	void ChunkContainer::clear()
	{
		m_chunks.clear();
	}
}