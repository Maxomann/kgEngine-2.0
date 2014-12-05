#include "ChunkSystem.h"
using namespace std;
using namespace sf;

namespace kg
{
	void ChunkSystem::init( Engine& engine, World& world )
	{
		m_connectToSignal( world.s_entity_added, &ChunkSystem::m_onEntityAddedToWorld );
		m_connectToSignal( world.s_entity_removed, &ChunkSystem::m_onEntityRemovedFromWorld );

		return;
	}

	void ChunkSystem::sfmlEvent( Engine& engine, const sf::Event& sfEvent )
	{
		return;
	}

	void ChunkSystem::update( Engine& engine, World& world )
	{
		return;
	}

	double ChunkSystem::getUpdateImportance() const
	{
		return ( double )id::SystemUpdateImportance::CHUNK_SYSTEM;
	}

	const std::string& ChunkSystem::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	int ChunkSystem::getPluginId() const
	{
		return ( int )id::SystemPluginId::CHUNK_SYSTEM;
	}

	void ChunkSystem::m_onEntityAddedToWorld( std::shared_ptr<Entity>& entity )
	{
		if( entity->hasComponent<Position>() )
		{
			m_connectToSignal( entity->getComponent<Position>()->s_changed,
							   std::function<void( const sf::Vector2i& )>(
							   std::bind( &ChunkSystem::m_onEntityPositionChanged,
							   this,
							   entity,
							   std::placeholders::_1 ) ) );

			m_refreshChunkInformation( entity );
		}
	}

	void ChunkSystem::m_refreshChunkInformation( std::shared_ptr<Entity>& entity )
	{
		auto newChunk = calculateChunkForPosition( entity->getComponent<Position>()->get() );

		auto findResult = m_entityData.find( entity );
		if( findResult != m_entityData.end() )
		{
			//entity is already registered
			auto oldChunk = findResult->second;
			if( oldChunk != newChunk )//only update if the chunk changed
			{
				//update entityData
				m_entityData.at( entity ) = newChunk;

				//remove from old chunk
				auto oldChunkData = m_chunkData.at( oldChunk.x ).at( oldChunk.y );
				oldChunkData.erase( std::find( oldChunkData.begin(), oldChunkData.end(), entity ) );

				//add to new chunk
				m_chunkData[newChunk.x][newChunk.y].push_back( entity );
			}
		}
		else
		{
			//entity is going to be registered
			m_entityData[entity] = newChunk;
			//add to new chunk
			m_chunkData[newChunk.x][newChunk.y].push_back( entity );
		}
	}

	void ChunkSystem::m_onEntityPositionChanged( std::shared_ptr<Entity>& entity, const sf::Vector2i& newPosition )
	{
		m_refreshChunkInformation( entity );
	}

	void ChunkSystem::m_onEntityRemovedFromWorld( std::shared_ptr<Entity>& entity )
	{
		auto currentChunk = m_entityData.at( entity );
		auto chunkData = m_chunkData.at( currentChunk.x ).at( currentChunk.y );
		chunkData.erase( std::find( chunkData.begin(), chunkData.end(), entity ) );
		m_entityData.erase( entity );
	}

	const ChunkSystem::EntityContainer& ChunkSystem::getEntitiesInChunk( const sf::Vector2i& chunk ) const
	{
		auto findResult = m_chunkData.find( chunk.x );
		if( findResult != m_chunkData.end() )
		{
			auto findResultY = findResult->second.find( chunk.y );
			if( findResultY != findResult->second.end() )
			{
				return findResultY->second;
			}
		}
		return m_emptyList;
	}

	sf::Vector2i ChunkSystem::calculateChunkForPosition( const sf::Vector2i& position )
	{
		auto chunk = position / CHUNK_SIZE;
		if( position.x < 0 )
			chunk.x -= 1;
		else
			chunk.x += 1;

		if( position.y < 0 )
			chunk.y -= 1;
		else
			chunk.y += 1;

		return chunk;
	}

	const sf::Vector2i& ChunkSystem::getChunkOfEntity( const std::shared_ptr<Entity>& entity )
	{
		auto& retVal = m_entityData.at( entity );
		return retVal;
		// out_of_range:
		// entity is not registered in ChunkSystem
		// possible reasons:
		// entity has not been added to World
		// or
		// entity is not in world anymore
		// or
		// its a bug -.-
	}

	const std::string ChunkSystem::PLUGIN_NAME = "ChunkSystem";
}