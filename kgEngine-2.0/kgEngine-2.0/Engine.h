#pragma once
#include "stdafx.h"
#include "PluginManager.h"
#include "Blueprint.h"
#include "ResourceManagement.h"

namespace kg
{
	struct DLL_EXPORT Engine
	{
		bool shouldTerminate = false;
		bool isPaused = false;//if true entities will not get updated
		sf::RenderWindow renderWindow;
		ResourceManagement resourceManagement;
		PluginManager pluginManager;
		blueprint::BlueprintManager blueprint;
	};
}
