#include "Chunk.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	std::string Chunk::getChunkSavename( const ChunkPosition& chunkPosition )
	{
		return "Chunk#" + to_string( chunkPosition.x ) + "#" + to_string( chunkPosition.y ) + "#" + to_string( chunkPosition.worldLayer );
	}

	void Chunk::addEntity( Entity* entity )
	{
		//update entityData
		auto transformationComponent = entity->getComponent<Transformation>();
		transformationComponent->setChunkPostion( m_position );

		m_entities.push_back( entity );
	}

	void Chunk::removeEntity( Entity* entity )
	{
		//update entityData
		auto transformationComponent = entity->getComponent<Transformation>();
		transformationComponent->removeChunkPosition();

		m_entities.erase( std::remove( m_entities.begin(), m_entities.end(), entity ), m_entities.end() );
	}

	std::string Chunk::getSavename() const
	{
		return getChunkSavename( m_position );
	}
}