#pragma once
#include "stdafx.h"
#include "Transformation.h"
#include "ChunkGeneratorSystem.h"
#include "GraphicsSystem.h"
#include "Chunk.h"
#include "ChunkIOOperationQueue.h"
#include "ChunkContainer.h"

namespace kg
{
	class ChunkSystem : public System, public CallbackReciever
	{
		std::shared_ptr<ConfigFile> m_configFile;
		struct ConfigValues
		{
			int chunkLoadRadiusAroundCamera;
			int chunkLoadCountPerFrame;
		}m_configValues;

		ChunkGeneratorSystem* r_chunkGeneratorSystem;

		// CHUNK LOAD STATE DATA:
		ChunkIOOperationQueue m_chunkIOOperationQueue;
		ChunkContainer m_chunks;//unloaded chunks will not be removed from this container

		int m_chunkLoadRadiusAroundCamera;
		void loadUnloadChunksAroundCameraPositions( Engine& engine, World& world, SaveManager& saveManager, const std::vector<Position2d>& cameraPositions );

		void ensureChunkLoaded( Engine& engine, World& world, SaveManager& saveManager, Chunk& chunk );
		//returns true if a chunk has been unloaded, false if it was already unloaded
		void ensureChunkUnloaded( Engine& engine, World& world, SaveManager& saveManager, Chunk& chunk );

		// ENTITY POSITION DATA:
		EntityPointerContainer container_null = EntityPointerContainer();//empty container for getEntitiesInChunk
		void m_refreshChunkInformation( Entity* entity );

		void m_onEntitiesAddedToWorld( const EntityPointerContainer& entities );
		void m_onEntitiesRemovedFromWorld( const EntityPointerContainer& entities );
		void m_onEntityPosition2dChanged( Entity* entity, const Position2d& newPosition );

		void m_onSavegameClosed();

	public:
		~ChunkSystem();

		virtual void init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile );

		virtual void sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime );

		virtual void saveChangesToConfigFile( std::shared_ptr<ConfigFile>& configFile ) override;

		virtual double getUpdateImportance() const;

		virtual const Plugin::Name& getPluginName() const;

		virtual Plugin::Id getPluginId()const;

		virtual const size_t& getRTTI_hash() const override;

		static const std::string PLUGIN_NAME;

		static const size_t type_hash;

		// returns a reference to the internal container
		const EntityPointerContainer& getEntitiesInChunk( const ChunkPosition& chunkPosition )const;

		void saveAllLoadedChunks( Engine& engine, World& world, SaveManager& saveManager );

		//configuration default values:
		static const std::string CHUNK_LOAD_RADIUS_AROUND_CAMERA;
		static const int CHUNK_LOAD_RADIUS_AROUND_CAMERA_DEFAULT;

		static const std::string CHUNK_LOAD_COUNT_PER_FRAME;
		static const int CHUNK_LOAD_COUNT_PER_FRAME_DEFAULT;
	};
}
