#pragma once
#include "stdafx.h"
#include "Position.h"

namespace kg
{
	class ChunkSystem : public System, public CallbackReciever
	{
		std::map < sf::Vector2i, std::list<std::shared_ptr<Entity>>> m_chunkData;
		std::map< std::shared_ptr<Entity>, sf::Vector2i > m_entityData;

		void m_refreshChunkInformation( std::shared_ptr<Entity>& entity );

		void m_onEntityAddedToWorld( int callbackId, std::shared_ptr<Entity>& entity );
		void m_onEntityRemovedFromWorld( int callbackId, std::shared_ptr<Entity>& entity );
		void m_onEntityPositionChanged( int callbackId, std::shared_ptr<Entity>& entity, const sf::Vector2i& newPosition );

	public:

		virtual void init( Engine& engine, World& world );

		virtual void sfmlEvent( const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world );

		virtual double getUpdateImportance() const;

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;

		static const std::string PLUGIN_NAME;

		static const int CHUNK_DIAMETER = 2000;//pixel


		// returns a reference to the internal container
		const std::list<std::shared_ptr<Entity>>& getEntitiesInChunk( const sf::Vector2i& chunk )const;

		// get entities from every chunk between from and to
		// returns a copy of the internal container
		/*std::list<std::shared_ptr<Entity>> getEntitiesInChunk( const sf::Vector2i& from,
															   const sf::Vector2i& to )const;*/

		static sf::Vector2i calculateChunkForPosition( const sf::Vector2i& position );
	};
}