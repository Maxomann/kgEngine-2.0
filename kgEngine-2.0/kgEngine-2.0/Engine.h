#pragma once
#include "stdafx.h"
#include "PluginManager.h"
#include "Blueprint.h"
#include "ResourceManagement.h"

namespace kg
{
	struct Engine
	{
		bool shouldTerminate = false;
		sf::RenderWindow renderWindow;
		ResourceManagement resourceManagement;
		PluginManager pluginManager;
		blueprint::BlueprintManager blueprint;
	};
}
