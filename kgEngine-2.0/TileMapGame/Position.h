#pragma once
#include "stdafx.h"

namespace kg
{
	class Position : public Component, public CallbackSender
	{
		sf::Vector2i m_position;//the CENTER of the Object

	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues );

		virtual void init( Engine& engine, ComponentManager& componentManager );

		virtual void update( Engine& engine, World& world, ComponentManager& componentManager );

		virtual double getUpdateImportance() const;

		virtual std::vector<size_t> getRequieredComponents() const;

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;

		void set( const sf::Vector2i& position );

		const sf::Vector2i& get()const;

		void move( const sf::Vector2i& offset );

		enum class CallbackId : int
		{
			CHANGED//T: const sf::Vector2i&
		};

		static const std::string PLUGIN_NAME;
	};
}
