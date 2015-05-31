#include "ChunkSystem.h"
using namespace std;
using namespace sf;

namespace kg
{
	void ChunkSystem::init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile )
	{
		m_connectToSignal( world.s_entity_added, &ChunkSystem::m_onEntityAddedToWorld );
		m_connectToSignal( world.s_entity_removed, &ChunkSystem::m_onEntityRemovedFromWorld );

		m_connectToSignal( saveManager.s_savegameClosed, &ChunkSystem::m_onSavegameClosed );

		m_configFile = configFile;

		//get config values
		m_configValues.chunkLoadRadiusAroundCamera = &configFile->getData( CHUNK_LOAD_RADIUS_AROUND_CAMERA );
		m_configValues.chunkLoadCountPerFrame = &configFile->getData( CHUNK_LOAD_COUNT_PER_FRAME );

		//set them if invalid ( and retrieve them a second time )
		if( !m_configValues.chunkLoadRadiusAroundCamera->size() )
			*m_configValues.chunkLoadRadiusAroundCamera = CHUNK_LOAD_RADIUS_AROUND_CAMERA_DEFAULT;
		if( !m_configValues.chunkLoadCountPerFrame->size() )
			*m_configValues.chunkLoadCountPerFrame = CHUNK_LOAD_COUNT_PER_FRAME_DEFAULT;

		m_chunkLoadRadiusAroundCamera = boost::lexical_cast< int >(*m_configValues.chunkLoadRadiusAroundCamera);
		m_chunkLoadCountPerFrame = boost::lexical_cast< int >(*m_configValues.chunkLoadCountPerFrame);

		return;
	}

	void ChunkSystem::sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent )
	{
		return;
	}

	void ChunkSystem::update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime )
	{
		auto cameras = world.getSystem<GraphicsSystem>()->getCameras();

		std::vector<sf::Vector2i> cameraPositions;
		for( const auto& camera : cameras )
			cameraPositions.push_back( camera->getComponent<Transformation>()->getPosition() );
		ensureChunksOnLoadUnloadListAroundCameraPositions( engine, world, saveManager, cameraPositions );

		loadAndUnloadChunksFromQueue( engine, world, saveManager );

		return;
	}

	double ChunkSystem::getUpdateImportance() const
	{
		return id::SystemUpdateImportance::CHUNK_SYSTEM;
	}

	const std::string& ChunkSystem::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id ChunkSystem::getPluginId() const
	{
		return id::SystemPluginId::CHUNK_SYSTEM;
	}

	void ChunkSystem::m_onEntityAddedToWorld( const std::shared_ptr<Entity>& entity )
	{
		if( entity->hasComponent<Transformation>() )
		{
			m_connectToSignal( entity->getComponent<Transformation>()->s_positionChanged,
							   function<void( const Vector2i& )>(
								   bind( &ChunkSystem::m_onEntityPositionChanged,
										 this,
										 weak_ptr<Entity>( entity ),
										 placeholders::_1 ) ) );

			m_refreshChunkInformation( entity );
		}
	}

	void ChunkSystem::m_refreshChunkInformation( const std::shared_ptr<Entity>& entity )
	{
		auto newChunk = calculateChunkForPosition( entity->getComponent<Transformation>()->getPosition() );

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
				auto& oldChunkData = m_chunkData.at( oldChunk.x ).at( oldChunk.y );
				oldChunkData.erase( find( oldChunkData.begin(), oldChunkData.end(), entity ) );

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

	void ChunkSystem::m_onEntityPositionChanged( weak_ptr<Entity>& entity, const sf::Vector2i& newPosition )
	{
		auto sharedEntity = entity.lock();
		if( sharedEntity )
			m_refreshChunkInformation( sharedEntity );
	}

	void ChunkSystem::m_onEntityRemovedFromWorld( const std::shared_ptr<Entity>& entity )
	{
		auto currentChunk = getChunkOfEntity( entity );
		auto& chunkData = m_chunkData.at( currentChunk.x ).at( currentChunk.y );
		auto it = std::find( chunkData.begin(), chunkData.end(), entity );
		chunkData.erase( it );
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
		auto chunk = position / Constants::CHUNK_SIZE;
		if( position.x < 0 )
			chunk.x -= 1;

		if( position.y < 0 )
			chunk.y -= 1;

		return chunk;
	}

	const size_t& ChunkSystem::getRTTI_hash() const
	{
		return type_hash;
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

	bool ChunkSystem::ensureChunkLoaded( Engine& engine, World& world, SaveManager& saveManager, const sf::Vector2i& chunkPosition )
	{
		if( !m_loadedChunks[chunkPosition.x][chunkPosition.y] )
		{
			// chunk not loaded
			if( !loadChunkFromFile( engine, world, saveManager, chunkPosition ) )
			{
				world.getSystem<ChunkGenerator>()->generateChunk( engine, world, chunkPosition );
				// chunk loaded
			}
			else
			{
				// chunk loaded
			}
			m_loadedChunks[chunkPosition.x][chunkPosition.y] = true;
			return true;
		}
		else
		{
			// chunk loaded
			return false;
		}
	}

	bool ChunkSystem::ensureChunkUnloaded( Engine& engine, World& world, SaveManager& saveManager, const sf::Vector2i& chunkPosition )
	{
		if( m_loadedChunks[chunkPosition.x][chunkPosition.y] )
		{
			// chunk loaded
			saveChunkToFile( engine, world, saveManager, chunkPosition );
			// remove entities in that chunk from world
			// copy entities here!(since removal of entities causes iterator invalidation
			const auto temp = getEntitiesInChunk( chunkPosition );
			for( const auto& entity : temp )
				world.removeEntity( entity );

			// chunk unloaded
			m_loadedChunks[chunkPosition.x][chunkPosition.y] = false;
			return true;
		}
		else
		{
			// chunk unloaded
			return false;
		}
	}

	bool ChunkSystem::loadChunkFromFile( Engine& engine, World& world, SaveManager& saveManager, const sf::Vector2i& chunkPosition )
	{
		return saveManager.loadEntitiesFromFile( engine, world, getChunkSavename( chunkPosition ) );
	}

	void ChunkSystem::saveChunkToFile( Engine& engine, World& world, SaveManager& saveManager, const sf::Vector2i& chunkPosition )
	{
		saveManager.saveEntitiesToFile( getChunkSavename( chunkPosition ), getEntitiesInChunk( chunkPosition ) );
	}

	void ChunkSystem::addChunkToLoadQueue( const sf::Vector2i& chunkPosition )
	{
		if( !m_loadedChunks[chunkPosition.x][chunkPosition.y] )
			if( find( m_chunkLoadQueue.begin(), m_chunkLoadQueue.end(), chunkPosition ) == m_chunkLoadQueue.end() )
			m_chunkLoadQueue.push_back( chunkPosition );

		//remove from unload queue
		auto itLoad = find( m_chunkUnloadQueue.begin(), m_chunkUnloadQueue.end(), chunkPosition );
		if( itLoad != m_chunkUnloadQueue.end() )
			m_chunkUnloadQueue.erase( itLoad );
	}

	void ChunkSystem::addChunkToUnloadQueue( const sf::Vector2i& chunkPosition )
	{
		if( m_loadedChunks[chunkPosition.x][chunkPosition.y] )
			if( find( m_chunkUnloadQueue.begin(), m_chunkUnloadQueue.end(), chunkPosition ) == m_chunkUnloadQueue.end() )
			m_chunkUnloadQueue.push_back( chunkPosition );

		//remove from load queue
		auto itLoad = find( m_chunkLoadQueue.begin(), m_chunkLoadQueue.end(), chunkPosition );
		if( itLoad != m_chunkLoadQueue.end() )
			m_chunkLoadQueue.erase( itLoad );
	}

	std::string ChunkSystem::getChunkSavename( const sf::Vector2i chunkPosition ) const
	{
		return "Chunk#" + to_string( chunkPosition.x ) + "#" + to_string( chunkPosition.y );
	}

	void ChunkSystem::ensureChunksOnLoadUnloadListAroundCameraPositions( Engine& engine, World& world, SaveManager& saveManager, const std::vector<sf::Vector2i>& cameraPositions )
	{
		vector<Vector2i> chunksToEnsureLoaded;

		vector<Vector2i> chunkPositions;
		for( const auto& el : cameraPositions )
			chunkPositions.push_back( calculateChunkForPosition( el ) );

		//add chunks that should be ensured to be loaded
		for( const auto& position : chunkPositions )
		{
			vector<Vector2i> temp;

			for( int x = (-1 * m_chunkLoadRadiusAroundCamera); x <= m_chunkLoadRadiusAroundCamera; ++x )
			{
				for( int y = (-1 * m_chunkLoadRadiusAroundCamera); y <= m_chunkLoadRadiusAroundCamera; ++y )
				{
					auto absoluteChunkPosition = Vector2i( position.x + x, position.y + y );

					if( length( sf::Vector2i( x, y ) ) < m_chunkLoadRadiusAroundCamera )
						temp.push_back( absoluteChunkPosition );
				}
			}

			//sort chunksToEnsureLoaded
			sort( begin( temp ), end( temp ), [&]( const Vector2i& lhs, const Vector2i& rhs )
			{
				return (length( rhs-position ) > length( lhs-position ));
			} );
			chunksToEnsureLoaded.insert( end( chunksToEnsureLoaded ), begin( temp ), end( temp ) );
		}

		/*unload chunks*/
		//for every chunks that is loaded atm and is not in chunksToEnsureLoaded: unload
		for( const auto& x : m_loadedChunks )
		{
			for( const auto& y : x.second )
			{
				if( y.second == true )//if chunk is loaded atm
				{
					sf::Vector2i chunkPosition( x.first, y.first );
					if( find( begin( chunksToEnsureLoaded ), end( chunksToEnsureLoaded ), chunkPosition )
						==
						end( chunksToEnsureLoaded ) )
					{
						//chunk is not on ensure loaded list
						addChunkToUnloadQueue( chunkPosition );
					}
				}
			}
		}

		/*load chunks*/
		for( const auto& el : chunksToEnsureLoaded )
			addChunkToLoadQueue( el );
	}

	void ChunkSystem::loadAndUnloadChunksFromQueue( Engine& engine, World& world, SaveManager& saveManager )
	{
		//unload
		for( int i = 0; i < m_chunkLoadCountPerFrame; )
		{
			if( m_chunkUnloadQueue.size() > 0 )
			{
				if( ensureChunkUnloaded( engine, world, saveManager, m_chunkUnloadQueue.front() ) )
					i++;
				m_chunkUnloadQueue.pop_front();
			}
			else break;
		}

		//load
		for( int i = 0; i < m_chunkLoadCountPerFrame; )
		{
			if( m_chunkLoadQueue.size() > 0 )
			{
				if( ensureChunkLoaded( engine, world, saveManager, m_chunkLoadQueue.front() ) )
					i++;
				m_chunkLoadQueue.pop_front();
			}
			else break;
		}

		m_chunkLoadQueue.clear();
		m_chunkUnloadQueue.clear();
	}

	void ChunkSystem::saveAllLoadedChunks( Engine& engine, World& world, SaveManager& saveManager )
	{
		for( const auto& x : m_loadedChunks )
			for( const auto& y : x.second )
			if( y.second )
			saveChunkToFile( engine, world, saveManager, Vector2i( x.first, y.first ) );
	}

	void ChunkSystem::m_onSavegameClosed()
	{
		m_chunkData.clear();
		m_entityData.clear();
		m_loadedChunks.clear();
	}

	const std::string ChunkSystem::CHUNK_LOAD_RADIUS_AROUND_CAMERA_DEFAULT = "5";

	const std::string ChunkSystem::CHUNK_LOAD_COUNT_PER_FRAME = "iChunkLoadCountPerFrame";

	const std::string ChunkSystem::CHUNK_LOAD_COUNT_PER_FRAME_DEFAULT = "1";

	const std::string ChunkSystem::CHUNK_LOAD_RADIUS_AROUND_CAMERA = "iChunkLoadRadiusAroundCamera";

	const std::string ChunkSystem::PLUGIN_NAME = "ChunkSystem";

	const size_t ChunkSystem::type_hash = getRuntimeTypeInfo<ChunkSystem>();

}
