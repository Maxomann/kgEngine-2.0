#pragma once
#include "stdafx.h"
#include "World.h"
#include "Engine.h"
#include "SaveManager.h"

namespace kg
{
	class Core
	{
		/// Connect function used in DLL-Files. The function in your DLL file has to look like this:
		/// extern "C" __declspec(dllexport) void kgConnect( PluginManager& pluginManager )
		typedef void( *CONNECT )(PluginManager&);

		Engine m_engine;
		SaveManager m_saveManager;
		World m_world;

		sf::Clock m_frameTimeClock;

		void loadPackages();

	public:
		void init();

		void update();
		bool shouldTerminate()const;

		static const int MIN_FRAME_TIME_IN_MILLISECONDS = 6;
	};
}
