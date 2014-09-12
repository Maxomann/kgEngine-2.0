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
		virtual void init()=0;

		virtual void update( ComponentManager& componentManager, Engine& engine, World& world )=0;

		//Id should be the same for all instances of the same component
		virtual int getId()=0;

		virtual double getUpdateImportance()const=0;
	};
}
