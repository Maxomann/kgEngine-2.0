#include "ChunkSystem.h"
using namespace std;
using namespace sf;

namespace kg
{
	ChunkSystem::~ChunkSystem()
	{
		m_chunkIOOperationQueue.completeAllOperations();
	}

	void ChunkSystem::init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile )
	{
		r_chunkGeneratorSystem = world.getSystem<ChunkGeneratorSystem>();

		m_connectToSignal( world.s_entities_added, &ChunkSystem::m_onEntitiesAddedToWorld );
		m_connectToSignal( world.s_entities_removed, &ChunkSystem::m_onEntitiesRemovedFromWorld );

		m_connectToSignal( saveManager.s_savegameClosed, &ChunkSystem::m_onSavegameClosed );

		m_configFile = configFile;

		//get config values
		m_configValues.chunkLoadRadiusAroundCamera = configFile->getDataReplaceIfInvalid( CHUNK_LOAD_RADIUS_AROUND_CAMERA, CHUNK_LOAD_RADIUS_AROUND_CAMERA_DEFAULT );
		m_configValues.chunkLoadCountPerFrame = configFile->getDataReplaceIfInvalid( CHUNK_LOAD_COUNT_PER_FRAME, CHUNK_LOAD_COUNT_PER_FRAME_DEFAULT );

		// apply config values
		m_chunkLoadRadiusAroundCamera = m_configValues.chunkLoadRadiusAroundCamera;
		m_chunkIOOperationQueue.setChunkIOCountPerFrame( m_configValues.chunkLoadCountPerFrame );

		return;
	}

	void ChunkSystem::sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent )
	{
		return;
	}

	void ChunkSystem::update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime )
	{
		auto cameras = world.getSystem<GraphicsSystem>()->getCameras();

		std::vector<Position2d> cameraPositions;
		for( const auto& camera : cameras )
			cameraPositions.push_back( camera->getComponent<Transformation>()->getPosition() );
		loadUnloadChunksAroundCameraPositions( engine, world, saveManager, cameraPositions );

		m_chunkIOOperationQueue.update();

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

	void ChunkSystem::m_onEntitiesAddedToWorld( const EntityPointerContainer& entities )
	{
		for( auto& entity : entities )
		{
			if( entity->hasComponent<Transformation>() )
			{
				m_connectToSignal( entity->getComponent<Transformation>()->s_position2dChanged,
								   function<void( const Position2d& )>(
									   bind( &ChunkSystem::m_onEntityPosition2dChanged,
											 this,
											 entity,
											 placeholders::_1 ) ) );

				m_refreshChunkInformation( entity );
			}
		}
	}

	void ChunkSystem::m_refreshChunkInformation( Entity* entity )
	{
		auto transformationComponent = entity->getComponent<Transformation>();

		auto newChunkPosition =
			Chunk::calculateChunkPositionForPosition2d( transformationComponent->getPosition() );

		auto oldChunkPosition = transformationComponent->getLastChunkPosition();
		if( oldChunkPosition )
		{
			//entity is already registered
			if( *oldChunkPosition != newChunkPosition )//only update if the chunk changed
			{
				//remove from old chunk
				m_chunks.getChunk( *oldChunkPosition ).removeEntity( entity );

				//add to new chunk
				m_chunks.getChunk( newChunkPosition ).addEntity( entity );
			}
		}
		else
		{
			//entity is going to be registered
			m_chunks.getChunk( newChunkPosition ).addEntity( entity );
		}
	}

	void ChunkSystem::m_onEntityPosition2dChanged( Entity* entity, const Position2d& newPosition )
	{
		m_refreshChunkInformation( entity );
	}

	void ChunkSystem::m_onEntitiesRemovedFromWorld( const EntityPointerContainer& entities )
	{
		for( auto& entity : entities )
		{
			if( entity->hasComponent<Transformation>() )
			{
				auto chunkPosition = entity->getComponent<Transformation>()->getLastChunkPosition();

				if( chunkPosition )
					m_chunks.getChunk( *chunkPosition ).removeEntity( entity );
			}
		}
	}

	const EntityPointerContainer& ChunkSystem::getEntitiesInChunk( const ChunkPosition& chunkPosition )const
	{
		if( m_chunks.doesChunkExist( chunkPosition ) )
			return m_chunks.getChunk_const( chunkPosition ).getEntities();
		else
			return container_null;
	}

	void ChunkSystem::saveChangesToConfigFile( std::shared_ptr<ConfigFile>& configFile )
	{
		return;
	}

	const size_t& ChunkSystem::getRTTI_hash() const
	{
		return type_hash;
	}

	void ChunkSystem::loadUnloadChunksAroundCameraPositions( Engine& engine, World& world, SaveManager& saveManager, const std::vector<Position2d>& cameraPositions )
	{
		vector<Position2d> cameraChunkPositions;
		for( const auto& el : cameraPositions )
			cameraChunkPositions.push_back( Chunk::calculateChunkPositionForPosition2d( el ) );

		vector<Position2d> chunksToEnsureLoaded;
		//add chunks that should be ensured to be loaded
		for( const auto& cameraChunkPosition : cameraChunkPositions )
		{
			vector<ChunkPosition> chunksToEnsureLoadedForCameraPosition;

			for( int x = (-1 * m_chunkLoadRadiusAroundCamera); x <= m_chunkLoadRadiusAroundCamera; ++x )
			{
				for( int y = (-1 * m_chunkLoadRadiusAroundCamera); y <= m_chunkLoadRadiusAroundCamera; ++y )
				{
					auto absoluteChunkPosition = ChunkPosition( cameraChunkPosition.x + x,
																cameraChunkPosition.y + y,
																cameraChunkPosition.worldLayer );

					if( length( sf::Vector2i( x, y ) ) < m_chunkLoadRadiusAroundCamera )
						chunksToEnsureLoadedForCameraPosition.push_back( absoluteChunkPosition );
				}
			}

			//sort chunksToEnsureLoaded
			sort( begin( chunksToEnsureLoadedForCameraPosition ),
				  end( chunksToEnsureLoadedForCameraPosition ),
				  [&]( const ChunkPosition& lhs,
					   const ChunkPosition& rhs )
			{
				auto lengthRhs = length( rhs.toPositionXY().toVector2i() - cameraChunkPosition.toPositionXY().toVector2i() );
				auto lengthLhs = length( lhs.toPositionXY().toVector2i() - cameraChunkPosition.toPositionXY().toVector2i() );

				return lengthRhs > lengthLhs;
			} );
			chunksToEnsureLoaded.insert( end( chunksToEnsureLoaded ),
										 begin( chunksToEnsureLoadedForCameraPosition ),
										 end( chunksToEnsureLoadedForCameraPosition ) );
		}

		/*unload chunks*/
		//for every chunks that is loaded atm and is not in chunksToEnsureLoaded: unload
		for( auto& chunk : m_chunks.getAllLoadedOrLoadingChunks() )
		{
			if( find( begin( chunksToEnsureLoaded ), end( chunksToEnsureLoaded ), chunk->getPosition() )
				==
				end( chunksToEnsureLoaded ) )
			{
				//chunk is not on ensure loaded list
				ensureChunkUnloaded( engine, world, saveManager, *chunk );
			}
		}

		/*load chunks*/
		for( const auto& position : chunksToEnsureLoaded )
			ensureChunkLoaded( engine, world, saveManager, m_chunks.getChunk( position ) );
	}

	void ChunkSystem::ensureChunkLoaded( Engine& engine, World& world, SaveManager& saveManager, Chunk& chunk )
	{
		if( chunk.getState() == Chunk::State::UNLOADED )
			m_chunkIOOperationQueue.addOperation( make_unique<ChunkLoadOperation>( engine, world, saveManager, *r_chunkGeneratorSystem, chunk ) );
		else if( chunk.getState() == Chunk::State::UNLOADING )
			m_chunkIOOperationQueue.abortAllOperationsOnChunk( chunk );
	}

	void ChunkSystem::ensureChunkUnloaded( Engine& engine, World& world, SaveManager& saveManager, Chunk& chunk )
	{
		if( chunk.getState() == Chunk::State::LOADED )
			m_chunkIOOperationQueue.addOperation( make_unique<ChunkUnloadOperation>( engine, world, saveManager, *r_chunkGeneratorSystem, chunk ) );
		else if( chunk.getState() == Chunk::State::LOADING )
			m_chunkIOOperationQueue.abortAllOperationsOnChunk( chunk );
	}

	void ChunkSystem::saveAllLoadedChunks( Engine& engine, World& world, SaveManager& saveManager )
	{
		for( auto chunk : m_chunks.getAllLoadedChunks() )
			m_chunkIOOperationQueue.addOperation( make_unique<ChunkSaveOperation>( engine, world, saveManager, *r_chunkGeneratorSystem, *chunk ) );
	}

	void ChunkSystem::m_onSavegameClosed()
	{
		m_chunkIOOperationQueue.completeAllOperations();
		m_chunks.clear();
	}

	const std::string ChunkSystem::CHUNK_LOAD_RADIUS_AROUND_CAMERA = "iChunkLoadRadiusAroundCamera";

	const int ChunkSystem::CHUNK_LOAD_RADIUS_AROUND_CAMERA_DEFAULT = 4;

	const std::string ChunkSystem::CHUNK_LOAD_COUNT_PER_FRAME = "iChunkLoadCountPerFrame";

	const int ChunkSystem::CHUNK_LOAD_COUNT_PER_FRAME_DEFAULT = 1;

	const std::string ChunkSystem::PLUGIN_NAME = "ChunkSystem";

	const size_t ChunkSystem::type_hash = getRuntimeTypeInfo<ChunkSystem>();
}