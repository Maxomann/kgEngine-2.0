#pragma once
#include "stdafx.h"
#include "Plugin.h"
#include "Engine.h"
#include "World.h"
#include "SaveManager.h"

namespace kg
{
	class GameStateManager;
	using Action = thor::Action;

	class DLL_EXPORT GameState : public Plugin, public CallbackReciever, public sf::NonCopyable
	{
	protected:
		Engine* r_engine;
		World* r_world;
		SaveManager* r_saveManager;

		sf::Time frameTime;

	public:

		void initReferences( Engine& engine, World& world, SaveManager& saveManager );
		void updateFrameTime( const sf::Time& frameTime );

		virtual void onInit() = 0;
		virtual void registerGui( tgui::Gui& gui ) = 0;
		virtual void registerInputCallbacks( InputManager& inputManager ) = 0;

		virtual void onUpdate( GameStateManager& gameStateManager ) = 0;

		virtual void removeInputCallbacks( InputManager& inputManager ) = 0;
		virtual void removeGui( tgui::Gui& gui ) = 0;
		virtual void onDestroy() = 0;
	};
}
