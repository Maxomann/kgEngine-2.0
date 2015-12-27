#include "ChunkSystem.h"
using namespace std;
using namespace sf;

namespace kg
{
	ChunkSystem::~ChunkSystem()
	{
		m_chunkIOOperationQueue.finishAllOperations();
	}

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

	void ChunkSystem::m_onEntityAddedToWorld( Entity* entity )
	{
		if( entity->hasComponent<Transformation>() )
		{
			m_connectToSignal( entity->getComponent<Transformation>()->s_positionChanged,
							   function<void( const Vector2i& )>(
								   bind( &ChunkSystem::m_onEntityPositionChanged,
										 this,
										 entity,
										 placeholders::_1 ) ) );

			m_refreshChunkInformation( entity );
		}
	}

	void ChunkSystem::m_refreshChunkInformation( Entity* entity )
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

	void ChunkSystem::m_onEntityPositionChanged( Entity* entity, const sf::Vector2i& newPosition )
	{
		m_refreshChunkInformation( entity );
	}

	void ChunkSystem::m_onEntityRemovedFromWorld( Entity* entity )
	{
		if( entity->hasComponent<Transformation>() )
		{
			auto currentChunk = getChunkOfEntity( entity );
			auto& chunkData = m_chunkData.at( currentChunk.x ).at( currentChunk.y );
			auto it = std::find( chunkData.begin(), chunkData.end(), entity );
			chunkData.erase( it );
			m_entityData.erase( entity );
		}
	}

	const ChunkSystem::EntityPointerContainer& ChunkSystem::getEntitiesInChunk( const sf::Vector2i& chunk ) const
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

	void ChunkSystem::saveChangesToConfigFile( std::shared_ptr<ConfigFile>& configFile )
	{
		return;
	}

	const size_t& ChunkSystem::getRTTI_hash() const
	{
		return type_hash;
	}

	const sf::Vector2i& ChunkSystem::getChunkOfEntity( Entity* entity )
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

	void ChunkSystem::saveOpenSavegame( Engine& engine, World& world, SaveManager& saveManager )
	{
		saveAllLoadedChunks( engine, world, saveManager );
		saveManager.saveSystems( world );
	}

	void ChunkSystem::ensureChunksOnLoadUnloadListAroundCameraPositions( Engine& engine, World& world, SaveManager& saveManager, const std::vector<sf::Vector3i>& cameraPositions )
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
				return (length( rhs - position ) > length( lhs - position ));
			} );
			chunksToEnsureLoaded.insert( end( chunksToEnsureLoaded ), begin( temp ), end( temp ) );
		}

		/*unload chunks*/
		//for every chunks that is loaded atm and is not in chunksToEnsureLoaded: unload
		for( const auto& x : m_chunks )
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

	bool ChunkSystem::ensureChunkLoaded( Engine& engine, World& world, SaveManager& saveManager, Chunk& chunk )
	{
		if( !chunk.isLoaded() )
			m_chunkIOOperationQueue.addOperation( make_unique<ChunkLoadOperation>( engine, world, saveManager, chunk ) );
	}

	bool ChunkSystem::ensureChunkUnloaded( Engine& engine, World& world, SaveManager& saveManager, Chunk& chunk )
	{
		if( chunk.isLoaded() )
			m_chunkIOOperationQueue.addOperation( make_unique<ChunkUnloadOperation>( engine, world, saveManager, chunk ) );
	}

	void ChunkSystem::saveAllLoadedChunks( Engine& engine, World& world, SaveManager& saveManager )
	{
		for( const auto& x : m_chunks )
		{
			for( const auto& y : x.second )
			{
				if( y.second )
					saveChunkToFile( engine, world, saveManager, Vector2i( x.first, y.first ) );
			}
		}
	}

	void ChunkSystem::m_onSavegameClosed()
	{
		m_chunkIOOperationQueue.finishAllOperations();
		m_chunkData.clear();
		m_entityData.clear();
		m_chunks.clear();
	}

	const std::string ChunkSystem::CHUNK_LOAD_RADIUS_AROUND_CAMERA_DEFAULT = "5";

	const std::string ChunkSystem::CHUNK_LOAD_COUNT_PER_FRAME = "iChunkLoadCountPerFrame";

	const std::string ChunkSystem::CHUNK_LOAD_COUNT_PER_FRAME_DEFAULT = "1";

	const std::string ChunkSystem::CHUNK_LOAD_RADIUS_AROUND_CAMERA = "iChunkLoadRadiusAroundCamera";

	const std::string ChunkSystem::PLUGIN_NAME = "ChunkSystem";

	const size_t ChunkSystem::type_hash = getRuntimeTypeInfo<ChunkSystem>();
}