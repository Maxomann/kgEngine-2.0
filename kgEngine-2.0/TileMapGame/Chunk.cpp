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

	bool Chunk::operator==( const Chunk& rhs ) const
	{
		return m_position == rhs.m_position;
	}

	Chunk::Chunk( const ChunkPosition& position )
		:m_position( position )
	{ }

	const ChunkPosition& Chunk::getPosition() const
	{
		return m_position;
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

	const World::EntityPointerContainer& Chunk::getEntities() const
	{
		return m_entities;
	}

	bool Chunk::isLoaded() const
	{
		return m_isLoaded;
	}

	void Chunk::setLoadState( bool isLoaded )
	{
		m_isLoaded = isLoaded;
	}

	std::string Chunk::getSavename() const
	{
		return getChunkSavename( m_position );
	}
}