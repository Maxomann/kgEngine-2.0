#pragma once
#include "stdafx.h"
#include "Plugin.h"

namespace kg
{
	struct Engine;
	class SystemManager;
	class World;

	class DLL_EXPORT System : public Plugin
	{
	public:
		virtual void init( Engine& engine, World& world ) = 0;

		// sfml event loop is forwarded before update() is called
		virtual void sfmlEvent( const sf::Event& sfEvent ) = 0;

		// components get updated before systems
		virtual void update( Engine& engine, World& world ) = 0;

		// the lower the importance, the earlier this component gets updated, initialized, sfmlForwarded
		virtual double getUpdateImportance()const = 0;
	};
}
