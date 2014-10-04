#pragma once
#include "stdafx.h"
#include "Component.h"
#include "_id.h"

namespace kg
{
	class PositionComponent: public Component
	{
		sf::Vector2i m_position = { 0, 0 };

	public:

		void setPosition( const sf::Vector2i& position );
		const sf::Vector2i& getPosition()const;

		virtual void init( Engine& engine, ComponentManager& componentManager );

		virtual void update( Engine& engine, World& world, ComponentManager& componentManager );

		virtual int getId();

		virtual double getUpdateImportance() const;

		virtual std::string getName();

	};
}