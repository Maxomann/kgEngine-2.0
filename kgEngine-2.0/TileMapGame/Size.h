#pragma once
#include "stdafx.h"
#include "Position.h"

namespace kg
{
	class Size : public Component, public CallbackSender, public CallbackReciever
	{
		sf::Vector2i m_size;

	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues );

		virtual void init( Engine& engine, ComponentManager& componentManager );

		virtual void update( Engine& engine, World& world, ComponentManager& componentManager );

		virtual double getUpdateImportance() const;

		virtual std::vector<size_t> getRequieredComponents() const;//Position

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;


		void set( const sf::Vector2i& size );
		sf::Vector2i get()const;

		enum class CallbackId : int
		{
			CHANGED//T: const sf::Vector2i& newSize
		};

		static const std::string BLUEPRINT_WIDTH;
		static const std::string BLUEPRINT_HEIGHT;

		static const std::string PLUGIN_NAME;

	};
}
