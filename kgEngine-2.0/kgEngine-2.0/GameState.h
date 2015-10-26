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

	// By Design only SafeCallbackReciever, since GameState can be deleted anytime
	class DLL_EXPORT GameState : public Plugin, public SafeCallbackReciever, public sf::NonCopyable
	{
	protected:
		Engine* r_engine;
		World* r_world;
		SaveManager* r_saveManager;
		GameStateManager* r_gameStateManager;

		sf::Time frameTime;

	public:

		void initReferences( Engine& engine, World& world, SaveManager& saveManager, GameStateManager& gameStateManager );
		void updateFrameTime( const sf::Time& frameTime );

		virtual void onInit() = 0;
		virtual void registerGui( tgui::Gui& gui ) = 0;
		virtual void registerInputCallbacks( InputManager& inputManager ) = 0;

		//A GameState MUST NOT delete or remove itself or other GameStates in this function
		virtual void onUpdate( ) = 0;

		virtual void removeInputCallbacks( InputManager& inputManager ) = 0;
		virtual void removeGui( tgui::Gui& gui ) = 0;
		virtual void onDestroy() = 0;
	};
}
