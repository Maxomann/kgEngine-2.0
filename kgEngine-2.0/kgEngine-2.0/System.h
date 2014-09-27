#pragma once
#include "stdafx.h"

namespace kg
{
	struct Engine;
	class SystemManager;
	class World;

	class System
	{
	public:
		virtual void init( Engine& engine, SystemManager& systemManager ) = 0;
		virtual void update( Engine& engine, World& world )=0;
		virtual double getUpdateImportance()const=0;
	};
}