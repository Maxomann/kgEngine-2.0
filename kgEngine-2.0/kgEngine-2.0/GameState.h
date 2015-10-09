#pragma once
#include "stdafx.h"
#include "Plugin.h"
#include "Engine.h"
#include "World.h"
#include "SaveManager.h"

namespace kg
{
	class DLL_EXPORT GameState : public PluginRTTI, public sf::NonCopyable
	{
	protected:
		Engine* r_engine;
		World* r_world;
		SaveManager* r_saveManager;

	public:

		void initReferences( Engine& engine, World& world, SaveManager& saveManager );

		virtual void initGui( tgui::Gui& gui ) = 0;
		virtual void initInputCallbacks( InputManager& inputManager ) = 0;

		virtual void removeGui( tgui::Gui& gui ) = 0;
		virtual void removeInputCallbacks( InputManager& inputManager ) = 0;
	};
}
