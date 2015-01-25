#pragma once
#include "stdafx.h"
#include "Transformation.h"
#include "ChunkGenerator.h"
#include "GraphicsSystem.h"

namespace kg
{
	class ChunkSystem : public System, public CallbackReciever
	{
		typedef std::vector<std::shared_ptr<Entity>> EntityContainer;

		std::shared_ptr<ConfigFile> m_configFile;
		struct ConfigValues
		{
			blueprint::Value chunkLoadRadiusAroundCamera;
		}m_configValues;

		// CHUNK LOAD STATE DATA:
		int chunkLoadRadiusAroundCamera;
		std::map<int, std::map<int, bool>> m_loadedChunks;
		std::string getChunkSavename( const sf::Vector2i chunkPosition )const;
		bool loadChunkFromFile( Engine& engine, World& world, SaveManager& saveManager, const sf::Vector2i& chunkPosition );//returns false if file did not exist
		void saveChunkToFile( Engine& engine, World& world, SaveManager& saveManager, const sf::Vector2i& chunkPosition );
		void ensureChunkLoaded( Engine& engine, World& world, SaveManager& saveManager, const sf::Vector2i& chunkPosition );
		void ensureChunkUnloaded( Engine& engine, World& world, SaveManager& saveManager, const sf::Vector2i& chunkPosition );
		void ensureChunksLoadedAroundCameraPositionsUnloadOther( Engine& engine, World& world, SaveManager& saveManager, const std::vector<sf::Vector2i>& cameraPositions );


		// ENTITY POSITION DATA:
		std::map< int, std::map<int, EntityContainer >> m_chunkData;//int x, int y
		std::map< std::shared_ptr<Entity>, sf::Vector2i > m_entityData;
		const EntityContainer m_emptyList = EntityContainer();//only for returnValue in getEntitiesInChunk()
		void m_refreshChunkInformation( std::shared_ptr<Entity>& entity );


		void m_onEntityAddedToWorld( std::shared_ptr<Entity>& entity );
		void m_onEntityRemovedFromWorld( std::shared_ptr<Entity>& entity );
		void m_onEntityPositionChanged( std::shared_ptr<Entity>& entity, const sf::Vector2i& newPosition );

		void m_onSavegameOpened( Engine& engine , World& world );

	public:

		virtual void init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile );

		virtual void sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime );

		virtual double getUpdateImportance() const;

		virtual const std::string& getPluginName() const;

		virtual Plugin::Id getPluginId()const;



		static const std::string PLUGIN_NAME;

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

	};
}
