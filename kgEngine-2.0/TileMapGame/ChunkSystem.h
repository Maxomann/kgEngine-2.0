#pragma once
#include "stdafx.h"
#include "Position.h"

namespace kg
{
	class ChunkSystem : public System, public CallbackReciever
	{
		typedef std::vector<std::shared_ptr<Entity>> EntityContainer;

		//int x, int y
		std::map< int, std::map<int, EntityContainer >> m_chunkData;
		std::map< std::shared_ptr<Entity>, sf::Vector2i > m_entityData;
		const EntityContainer m_emptyList = EntityContainer();//only for returnValue in getEntitiesInChunk()

		void m_refreshChunkInformation( std::shared_ptr<Entity>& entity );

		void m_onEntityAddedToWorld( std::shared_ptr<Entity>& entity );
		void m_onEntityRemovedFromWorld( std::shared_ptr<Entity>& entity );
		void m_onEntityPositionChanged( std::shared_ptr<Entity>& entity, const sf::Vector2i& newPosition );

	public:

		virtual void init( Engine& engine, World& world, std::shared_ptr<ConfigFile>& configFile );

		virtual void sfmlEvent( Engine& engine, const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world );

		virtual double getUpdateImportance() const;

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;

		static const std::string PLUGIN_NAME;

		// returns a reference to the internal container
		const EntityContainer& getEntitiesInChunk( const sf::Vector2i& chunk )const;

		//entity must be registered in world
		const sf::Vector2i& getChunkOfEntity( const std::shared_ptr<Entity>& entity );

		// get entities from every chunk between from and to
		// returns a copy of the internal container
		/*std::list<std::shared_ptr<Entity>> getEntitiesInChunk( const sf::Vector2i& from,
															   const sf::Vector2i& to )const;*/

		// returns the position in chunks for the position of an entity
		// never returns {0,0}
		static sf::Vector2i calculateChunkForPosition( const sf::Vector2i& position );

		static const int CHUNK_SIZE = 640;
	};
}
