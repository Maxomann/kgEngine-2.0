#pragma once
#include "stdafx.h"
#include "PluginManager.h"
#include "Blueprint.h"
#include "ResourceManager.h"
#include "RandomNumberGenerator.h"
#include "InputManager.h"
#include "Console.h"
#include "Callback.h"

namespace kg
{
	/// Collection of basic classes that don't depend on a concrete instance of world
	///
	/// @author	Kay
	/// @date	26.01.2015

	struct DLL_EXPORT Engine
	{
		// true if the program should terminate.
		bool shouldTerminate = false;
		// if true entities will not get updated.
		bool isPaused = false;
		// The SFML RenderWindow.
		sf::RenderWindow renderWindow;
		// Manager for plugins.
		PluginManager pluginManager;
		// The BlueprintManager.
		blueprint::BlueprintManager blueprint;
		// Manager for resources loaded from the hard drive.
		ResourceManager resourceManager;
		// RandomNumberGenerator.
		RandomNumberGenerator randomNumberGenerator;
		// InputManager
		InputManager inputManager;
		// Console
		Console console;
	};
}
