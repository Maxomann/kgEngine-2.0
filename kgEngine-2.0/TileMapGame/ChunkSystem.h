#pragma once
#include "stdafx.h"

namespace kg
{
	class ChunkSystem : public System, public CallbackReciever
	{
		void m_refreshChunkInformation( std::shared_ptr<Entity>& entity );

		void m_onEntityAddedToWorld( int callbackId, std::shared_ptr<Entity>& entity );
		void m_onEntityPositionChanged( int callbackId, const sf::Vector2i& newPosition );

	public:

		virtual void init( Engine& engine, World& world );

		virtual void sfmlEvent( const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world );

		virtual double getUpdateImportance() const;

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;

		static const std::string PLUGIN_NAME;

		static const int CHUNK_DIAMETER = 2000;//pixel

	};
}