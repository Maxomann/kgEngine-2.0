#pragma once
#include "stdafx.h"
#include "Position.h"

namespace kg
{
	class Transformation : public Component, public CallbackReciever
	{
		sf::FloatRect m_globalBounds;

		Position2d m_position;
		boost::optional<ChunkPosition> m_chunkPosition;//if not available it has not been set yet
		float m_rotation = 0;//in degree
		sf::Vector2i m_size;

		void onLoadSaveInformation( const std::vector<std::string>& information );
		std::vector<std::string> onWriteSaveInformation();

		void recalculateGlobalBounds();

	public:
		virtual void preInit( Engine& engine, const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues ) override;

		virtual void init( Engine& engine, const World& world, const ComponentManager& thisEntity ) override;

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime ) override;

		virtual double getUpdateImportance() const override;

		virtual std::vector<Plugin::Id> getRequieredComponents()const override;

		virtual const Plugin::Name& getPluginName() const override;

		virtual Plugin::Id getPluginId() const override;

		virtual const size_t& getRTTI_hash() const override;

		//Position
		void setPosition( const Position2d& position );
		const Position2d& getPosition()const;

		void moveXYsetWorldLayer( const sf::Vector2i& offsetXY, int worldLayer );

		void setPositionXY( const PositionXY& position );
		PositionXY getPositionXY()const;
		void moveXY( const sf::Vector2i& offsetXY );

		void setWorldLayer( int layer );
		int getWorldLayer()const;

		// return boost::none if it has not been initialized
		// if an entity gets removed from world the LastChunkPositon will remain the same
		const boost::optional<ChunkPosition>& getLastChunkPosition()const;
		//DO NOT CALL THIS. IT WILL CAUSE ERRORS. Only Chunk should call this
		void setChunkPostion( ChunkPosition chunkPosition );

		//Rotation
		void setRotation( const float rotationInDegree );

		float getRotationInDegree()const;

		void rotate( const float offsetInDegree );

		//Size
		void setSize( const sf::Vector2i& size );

		sf::Vector2i getSize()const;

		//GlobalBounds
		sf::FloatRect getGlobalBounds()const;/*thread safe*/
		bool intersects( const sf::FloatRect& rect )const;

		static const std::string PLUGIN_NAME;

		static const size_t type_hash;

	signals:
		Signal<> s_transformationChanged;//any of: Position, Rotation, Size

		Signal<const Position2d&> s_position2dChanged;

		Signal<const float&> s_rotationChanged;
		Signal<const sf::Vector2i&> s_sizeChanged;

		static const std::string BLUEPRINT_WIDTH;
		static const std::string BLUEPRINT_HEIGHT;
	};
}
