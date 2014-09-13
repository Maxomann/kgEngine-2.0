#pragma once
#include "stdafx.h"

namespace kg
{
	struct Engine;
	class ComponentManager;
	class World;

	class Component
	{
	public:
		virtual void init(Engine& engine, ComponentManager& componentManager) =0;

		virtual void update( Engine& engine, World& world, ComponentManager& componentManager )=0;

		//Id should be the same for all instances of the same component
		virtual int getId()=0;

		virtual double getUpdateImportance()const=0;

	};
}
