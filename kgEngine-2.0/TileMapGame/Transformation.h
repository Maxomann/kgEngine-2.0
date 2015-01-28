#pragma once
#include "stdafx.h"

namespace kg
{
	class Transformation : public Component, public CallbackReciever
	{
		sf::Vector2i m_position;
		float m_rotation=0;//in degree
		sf::Vector2i m_size;
		int m_zValue = 0;


		void onLoadSaveInformation( const std::vector<std::string>& information );

		std::vector<std::string> onWriteSaveInformation();


	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues ) override;

		virtual void init( Engine& engine, ComponentManager& thisEntity ) override;

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime ) override;

		virtual double getUpdateImportance() const override;

		virtual std::vector<size_t> getRequieredComponents() const override;

		virtual const std::string& getPluginName() const override;

		virtual Plugin::Id getPluginId() const override;


		//Position
		void setPosition( const sf::Vector2i& position );

		const sf::Vector2i& getPosition()const;

		void move( const sf::Vector2i& offset );


		//Rotation
		void setRotation( const float rotationInDegree );

		float getRotationInDegree()const;

		void rotate( const float offsetInDegree );


		//Size
		void setSize( const sf::Vector2i& size );

		sf::Vector2i getSize()const;


		//zValue
		int getZValue()const;
		void setZValue( int zValue );


		//GlobalBounds
		const sf::FloatRect getGlobalBounds()const;


		static const std::string PLUGIN_NAME;

	signals:
		Signal<> s_transformationChanged;//any of: Position, Rotation, Size

		Signal<const sf::Vector2i&> s_positionChanged;
		Signal<const float&> s_rotationChanged;
		Signal<const sf::Vector2i&> s_sizeChanged;


		static const std::string BLUEPRINT_WIDTH;
		static const std::string BLUEPRINT_HEIGHT;
		static const std::string BLUEPRINT_ZVALUE;
	};
}
