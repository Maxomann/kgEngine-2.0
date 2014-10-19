#pragma once
#include "stdafx.h"
#include "Plugin.h"

namespace kg
{
	struct Engine;
	class ComponentManager;
	class World;
	namespace blueprint
	{
		class Entity;
		class Value;
	}

	class DLL_EXPORT Component : public Plugin
	{
	public:
		// get values from blueprint Entity
		virtual void preInit( const std::map<std::string, blueprint::Value>& blueprintValues ) = 0;

		// it is guaranteed that all components of the entity are loaded at this point
		// their init function is not guaranteed to have been called
		virtual void init( Engine& engine, ComponentManager& componentManager ) = 0;

		virtual void update( Engine& engine, World& world, ComponentManager& componentManager ) = 0;

		//the lower the importance, the earlier this component gets updated
		virtual double getUpdateImportance()const = 0;

		virtual std::vector<size_t> getRequieredComponents()const = 0;
	};
}
