#pragma once
#include "stdafx.h"
#include "Plugin.h"

namespace kg
{
	struct Engine;
	class SystemManager;
	class World;
	class ConfigFile;

	class DLL_EXPORT System : public Plugin
	{
	public:
		// other systems are not guaranteed to be initialized
		// configFile is only used for values of this system
		virtual void init( Engine& engine, World& world, std::shared_ptr<ConfigFile>& configFile ) = 0;

		// sfml event loop is forwarded before update() is called
		virtual void sfmlEvent( Engine& engine, const sf::Event& sfEvent ) = 0;

		// components get updated before systems
		virtual void update( Engine& engine, World& world ) = 0;

		// the lower the importance, the earlier this component gets updated, initialized, sfmlForwarded
		virtual double getUpdateImportance()const = 0;
	};
}
