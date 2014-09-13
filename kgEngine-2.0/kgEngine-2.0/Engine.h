#pragma once
#include "stdafx.h"
#include "PluginManager.h"

namespace kg
{
	struct Engine
	{
		bool shouldTerminate = false;
		sf::RenderWindow renderWindow;
		PluginManager pluginManager;
	};
}
