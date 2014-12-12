#pragma once
#include "stdafx.h"

namespace kg
{
	class Position : public Component
	{
		sf::Vector2i m_position;//the CENTER of the Object

	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues );

		virtual void init( Engine& engine, ComponentManager& thisEntity );

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime );

		virtual double getUpdateImportance() const;

		virtual std::vector<size_t> getRequieredComponents() const;

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;

		void set( const sf::Vector2i& position );

		const sf::Vector2i& get()const;

		void move( const sf::Vector2i& offset );

	signals:
		Signal<const sf::Vector2i&> s_changed;

		static const std::string PLUGIN_NAME;
	};
}
