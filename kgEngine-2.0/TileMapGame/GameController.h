#pragma once
#include "stdafx.h"
#include "ChunkSystem.h"
#include "GraphicsSystem.h"

namespace kg
{
	class GameController : public System, public CallbackReciever
	{
	private:
		Engine* r_engine;
		World* r_world;
		SaveManager* r_saveManager;
		GraphicsSystem* r_graphicsSystem;

		std::weak_ptr<Entity> m_camera;
		float m_cameraZoomFactor = 1;
		std::shared_ptr<Entity> m_getValidCamera();

		sf::Int32 lastFrameTimeInMilliseconds = 0;

		void saveOpenSavegame( Engine& engine, World& world, SaveManager& saveManager );

		void movePlayer( sf::Vector2i distance );

		void m_onSavegameOpened( Engine& engine );

	public:
		virtual void init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile );

		virtual void sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime );

		virtual double getUpdateImportance() const;

		virtual const Plugin::Name& getPluginName() const;

		virtual Plugin::Id getPluginId()const;

		virtual const size_t& getRTTI_hash() const override;

		static const std::string PLUGIN_NAME;

		static const size_t type_hash;

		//Input Callbacks
	private:
		void registerInputCallbacks( InputManager& inputManager );

		void shutDown();
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
