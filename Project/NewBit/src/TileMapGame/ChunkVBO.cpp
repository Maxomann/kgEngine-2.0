#include "ChunkVBO.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	ChunkVBO::ChunkVBO( unsigned int vertexCapacity, GLenum usage )
		:SpriteVBO( vertexCapacity, usage )
	{ }

	void ChunkVBO::setChunkPosition( const PositionXY& position )
	{
		m_chunkPositionXY = position;
	}

	const PositionXY& ChunkVBO::getChunkPosition() const
	{
		return m_chunkPositionXY;
	}

	/*const bool ChunkVBO::operator==( const ChunkVBO& other ) const
	{
		if( m_chunkPositionXY == other.m_chunkPositionXY && getTexture() == other.getTexture() )
			return true;
	}*/
}