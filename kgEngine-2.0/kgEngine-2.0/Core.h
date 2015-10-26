#pragma once
#include "stdafx.h"
#include "World.h"
#include "Engine.h"
#include "SaveManager.h"
#include "GameStateManager.h"

namespace kg
{
	class Core
	{
		// Connect function used in DLL-Files. The function in your DLL file has to look like this:
		// extern "C" __declspec(dllexport) void kgConnect( kg::PluginManager& pluginManager )
		typedef void( *CONNECT )(PluginManager&);

		Engine m_engine;
		SaveManager m_saveManager;
		World m_world;
		GameStateManager m_gameStateManager;

		sf::Clock m_frameTimeClock;

		void loadPackages();

	public:
		Core();

		void init();

		void update();
		bool shouldTerminate()const;

		void terminate();

		static const int MIN_FRAME_TIME_IN_MILLISECONDS = 6;
	};
}
