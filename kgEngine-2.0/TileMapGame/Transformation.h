#pragma once
#include "stdafx.h"
#include "Position.h"

namespace kg
{
	class Transformation : public Component, public CallbackReciever
	{
		sf::FloatRect m_globalBounds;

		Position m_position;
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
		void setPosition( const Position& position );
		const Position& getPosition()const;

		void setPositionXY( const sf::Vector2i& position );
		sf::Vector2i getPositionXY()const;
		void moveXY( const sf::Vector2i& offsetXY );

		void setZValue( int zValue );
		int getZValue()const;

		void setPositionXYZ( const sf::Vector3i& position );
		sf::Vector3i getPositionXYZ()const;

		int getWorldLayer()const;
		void setWorldLayer( int layer );

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
		sf::Vector3i getXYZValues()const;

		static const std::string PLUGIN_NAME;

		static const size_t type_hash;

	signals:
		Signal<> s_transformationChanged;//any of: Position, Rotation, Size

		Signal<const Position&> s_positionChanged;
		Signal<const sf::Vector2i&> s_positionXYChanged;
		Signal<const sf::Vector3i&> s_positionXYZChanged;
		Signal<const float&> s_rotationChanged;
		Signal<const sf::Vector2i&> s_sizeChanged;

		static const std::string BLUEPRINT_WIDTH;
		static const std::string BLUEPRINT_HEIGHT;
	};
}
