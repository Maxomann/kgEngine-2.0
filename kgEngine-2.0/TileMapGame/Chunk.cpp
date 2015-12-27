#include "Chunk.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	std::string Chunk::getChunkSavename( const Position2d& chunkPosition )
	{
		return "Chunk#" + to_string( chunkPosition.x ) + "#" + to_string( chunkPosition.y ) + "#" + to_string( chunkPosition.worldLayer );
	}

	std::string Chunk::getSavename() const
	{
		return getChunkSavename( m_position );
	}
}