#pragma once
#include "stdafx.h"
#include "Component.h"
#include "Blueprint.h"
#include "_id.h"
#include "ComponentManager.h"

namespace kg
{
	class PositionComponent :public Component
	{
		sf::Vector2i m_position;

	public:
		virtual void preInit( const std::map<std::string, blueprint::Value>& blueprintValues )
		{
			//m_position.x = blueprintValues.at( "x" ).toIntWithUnit().first;
			//m_position.y = blueprintValues.at( "y" ).toIntWithUnit().first;
		}

		virtual void init( Engine& engine, ComponentManager& componentManager )
		{
			return;
		}

		virtual void update( Engine& engine, World& world, ComponentManager& componentManager )
		{
			return;
		}

		virtual double getUpdateImportance() const
		{
			return ComponentUpdateImportance::PositionComponent;
		}

		const sf::Vector2i& getPosition()const
		{
			return m_position;
		}

		void setPosition( const sf::Vector2i position )
		{
			m_position = position;
		}

	};

	class SpriteComponent :public Component, public sf::Drawable
	{
		sf::Texture m_texture;
		sf::Sprite m_sprite;

	public:
		virtual void preInit( const std::map<std::string, blueprint::Value>& blueprintValues )
		{
			m_texture.loadFromFile( blueprintValues.at( "path" ).toString() );
		}

		virtual void init( Engine& engine, ComponentManager& componentManager )
		{
			m_sprite.setTexture( m_texture );
		}

		virtual void update( Engine& engine, World& world, ComponentManager& componentManager )
		{
			m_sprite.setPosition( sf::Vector2f( componentManager.getComponent<PositionComponent>()->getPosition() ) );
			return;
		}

		virtual double getUpdateImportance() const
		{
			return ComponentUpdateImportance::SpriteComponent;
		}

		virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const
		{
			target.draw( m_sprite, states );
		}

	};
}
