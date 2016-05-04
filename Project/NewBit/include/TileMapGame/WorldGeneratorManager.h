#pragma once
#include "stdafx.h"

namespace kg
{
	class WorldGeneratorManager
	{
	public:
		//loads WorldGenerators from PluginManager
		void init( Engine& engine );
	};
}