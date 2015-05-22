#pragma once
#include "stdafx.h"
#include "Transformation.h"
#include "ChunkGenerator.h"
#include "GraphicsSystem.h"

#define LOAD_CHUNKS_ONLY_ONCE 0
#define DONT_UNLOAD_CHUNKS 0
#define UNLOAD_ALL_CHUNKS_EVERY_FRAME 0

namespace kg
{
	class ChunkSystem : public System, public CallbackReciever
	{
		typedef std::unordered_set<std::shared_ptr<Entity>> EntityContainer;

		std::shared_ptr<ConfigFile> m_configFile;
		struct ConfigValues
		{
			std::string* chunkLoadRadiusAroundCamera;
			std::string* chunkLoadCountPerFrame;
		}m_configValues;

		// CHUNK LOAD STATE DATA:
		int m_chunkLoadRadiusAroundCamera;
		std::map<int, std::map<int, bool>> m_loadedChunks;
		std::string getChunkSavename( const sf::Vector2i chunkPosition )const;
		bool loadChunkFromFile( Engine& engine, World& world, SaveManager& saveManager, const sf::Vector2i& chunkPosition );//returns false if file did not exist
		void saveChunkToFile( Engine& engine, World& world, SaveManager& saveManager, const sf::Vector2i& chunkPosition );

		void addChunkToLoadQueue( const sf::Vector2i& chunkPosition );
		void addChunkToUnloadQueue( const sf::Vector2i& chunkPosition );
		void ensureChunksOnLoadUnloadListAroundCameraPositions( Engine& engine, World& world, SaveManager& saveManager, const std::vector<sf::Vector2i>& cameraPositions );

		int m_chunkLoadCountPerFrame;
		std::list<sf::Vector2i> m_chunkLoadQueue;
		std::list<sf::Vector2i> m_chunkUnloadQueue;
		void ensureChunkLoaded( Engine& engine, World& world, SaveManager& saveManager, const sf::Vector2i& chunkPosition );
		void ensureChunkUnloaded( Engine& engine, World& world, SaveManager& saveManager, const sf::Vector2i& chunkPosition );
		void loadAndUnloadChunksFromQueue( Engine& engine, World& world, SaveManager& saveManager );


		// ENTITY POSITION DATA:
		std::map< int, std::map<int, EntityContainer >> m_chunkData;//int x, int y
		std::map< std::shared_ptr<Entity>, sf::Vector2i > m_entityData;
		const EntityContainer m_emptyList = EntityContainer();//only for returnValue in getEntitiesInChunk()
		void m_refreshChunkInformation( const std::shared_ptr<Entity>& entity );

		void m_onEntityAddedToWorld( const std::shared_ptr<Entity>& entity );
		void m_onEntityRemovedFromWorld( const std::shared_ptr<Entity>& entity );
		void m_onEntityPositionChanged( std::weak_ptr<Entity>& entity, const sf::Vector2i& newPosition );

		void m_onSavegameClosed();

	public:

		virtual void init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile );

		virtual void sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime );

		virtual double getUpdateImportance() const;

		virtual const Plugin::Name& getPluginName() const;

		virtual Plugin::Id getPluginId()const;

		static const std::string PLUGIN_NAME;

		static const size_t type_hash;

		// returns a reference to the internal container
		const EntityContainer& getEntitiesInChunk( const sf::Vector2i& chunk )const;

		//entity must be registered in world
		const sf::Vector2i& getChunkOfEntity( const std::shared_ptr<Entity>& entity );

		// get entities from every chunk between from and to
		// returns a copy of the internal container
		/*std::list<std::shared_ptr<Entity>> getEntitiesInChunk( const sf::Vector2i& from,
															   const sf::Vector2i& to )const;*/

		void saveAllLoadedChunks( Engine& engine, World& world, SaveManager& saveManager );

		// returns the position in chunks for the position of an entity
		static sf::Vector2i calculateChunkForPosition( const sf::Vector2i& position );

		//configuration default values:
		static const std::string CHUNK_LOAD_RADIUS_AROUND_CAMERA;
		static const std::string CHUNK_LOAD_RADIUS_AROUND_CAMERA_DEFAULT;

		static const std::string CHUNK_LOAD_COUNT_PER_FRAME;
		static const std::string CHUNK_LOAD_COUNT_PER_FRAME_DEFAULT;

	};
}
