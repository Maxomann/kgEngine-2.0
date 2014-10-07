#pragma once
#include "stdafx.h"

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

	class Component
	{
	public:
		// get values from blueprint Entity
		virtual void preInit( const std::map<std::string, blueprint::Value>& blueprintValues ) = 0;

		virtual void init(Engine& engine, ComponentManager& componentManager) =0;

		virtual void update( Engine& engine, World& world, ComponentManager& componentManager )=0;

		//Id should be the same for all instances of the same component
		//virtual int getId()=0;

		//virtual std::string getName() = 0;

		//the lower the importance, the earlier this component gets updated
		virtual double getUpdateImportance()const=0;

	};
}
