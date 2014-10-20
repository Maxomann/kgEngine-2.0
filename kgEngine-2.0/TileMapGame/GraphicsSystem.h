#pragma once
#include "stdafx.h"
#include "Graphics.h"

namespace kg
{
	class GraphicsSystem : public System
	{
// 		sf::Vector2i position;
// 		std::shared_ptr<Entity> myEntity;

	public:
		virtual void init( Engine& engine, SystemManager& systemManager );

		virtual void sfmlEvent( const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world );

		virtual double getUpdateImportance() const;

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;

		static const std::string PLUGIN_NAME;

	};
}