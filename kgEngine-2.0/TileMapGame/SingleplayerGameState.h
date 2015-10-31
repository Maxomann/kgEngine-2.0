#pragma once
#include "stdafx.h"
#include "ChunkSystem.h"
#include "GraphicsSystem.h"
#include "ConsoleGameState.h"

namespace kg
{
	class SingleplayerGameState : public GameState
	{
		GraphicsSystem* r_graphicsSystem;

		PauseLock::Ptr m_pauseLock;

		float m_cameraZoomFactor = 1;
		Entity* m_getValidCamera();

		void movePlayer( sf::Vector2i distance );

		void m_onSavegameOpened( Engine& engine, World& world );

	public:
		virtual void onInit() override;

		virtual void registerGui( tgui::Gui& gui ) override;

		virtual void registerInputCallbacks( InputManager& inputManager ) override;

		virtual void onUpdate( ) override;

		virtual void removeInputCallbacks( InputManager& inputManager ) override;

		virtual void removeGui( tgui::Gui& gui ) override;

		virtual void onDestroy() override;


		virtual const Plugin::Name& getPluginName() const override;

		virtual Plugin::Id getPluginId() const override;


		static const std::string PLUGIN_NAME;


		//Input Callbacks
	private:
		void reloadSave();

		void pause();

		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();
		void moveUpFast();
		void moveDownFast();
		void moveLeftFast();
		void moveRightFast();
		void moveUpSlow();
		void moveDownSlow();
		void moveLeftSlow();
		void moveRightSlow();

		void zoomIn();
		void zoomOut();

	};
}
