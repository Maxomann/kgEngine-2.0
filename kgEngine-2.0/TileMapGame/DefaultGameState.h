#pragma once
#include "stdafx.h"
#include "ChunkSystem.h"
#include "SingleplayerGameState.h"

namespace kg
{
	class DefaultGameState : public GameState
	{
		bool m_singleplayerGameStateLaunched = false;

	public:
		virtual void onInit() override;

		virtual void registerGui( tgui::Gui& gui ) override;

		virtual void registerInputCallbacks( InputManager& inputManager ) override;

		virtual void onUpdate( GameStateManager& gameStateManager ) override;

		virtual void removeInputCallbacks( InputManager& inputManager ) override;

		virtual void removeGui( tgui::Gui& gui ) override;

		virtual void onDestroy() override;


		virtual const Plugin::Name& getPluginName() const override;

		virtual Plugin::Id getPluginId() const override;


		static const std::string PLUGIN_NAME;

		//Input Callbacks
	private:
		void shutDown();
	};
}