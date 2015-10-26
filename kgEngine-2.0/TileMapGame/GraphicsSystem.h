#pragma once
#include "stdafx.h"
#include "id.h"
#include "Camera.h"

namespace kg
{
	class GraphicsSystem : public System, public CallbackReciever
	{
	private:
		bool m_drawingShouldTerminate = false;

		//mutable boost::mutex m_syncMutex;
		//bool m_threadHasToWait = true;

		std::vector<std::tuple<sf::Vector3i, Entity*, Graphics*>> m_toDrawSorted;
		boost::mutex m_toDrawSortedMutex;

		EntityManager::EntityPointerContainer m_addedEntitiesCopy;

		mutable boost::mutex m_cameraContainerMutex;
		EntityManager::EntityPointerContainer m_cameras;

		mutable boost::mutex m_addedEntitiesCopyMutex;

		bool m_drawingIsActive = false;

		void drawingThreadFunction();

		///////////////////////////////////

		std::shared_ptr<ConfigFile> m_configFile;
		struct ConfigValues
		{
			std::string* antialiasing;
			std::string* fullscreen;
			std::string* window_resx;
			std::string* window_resy;
			std::string* vsync;
			std::string* window_name;
			std::string* drawDistance;
		}m_configValues;

		sf::RenderWindow* r_renderWindow;
		tgui::Gui* r_gui;

		void m_initCameras( Engine& engine, World& world );

		EntityManager::EntityPointerContainer m_addedEntities;
		EntityManager::EntityPointerContainer m_removedEntities;
		void m_onEntityAddedToWorld( Entity* entity );
		void m_onEntityRemovedFromWorld( Entity* entity );
		void m_onRemoveEntitiesFromRemoveList();

		void m_onSavegameOpened( Engine& engine, World& world );
		void m_onSavegameClosed();

		void m_launchDrawingThread( sf::RenderWindow& renderWindow );
		void m_terminateDrawingThread();
		int m_drawingThreadFrameTime = -1;

		mutable boost::mutex m_drawDistanceMutex;
		unsigned int m_drawDistance=0;

	public:
		GraphicsSystem();

		virtual void init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile );

		virtual void sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime );

		virtual void destroy( Engine& engine, std::shared_ptr<ConfigFile>& configFile ) override;

		virtual double getUpdateImportance() const;

		virtual const Plugin::Name& getPluginName() const;

		virtual Plugin::Id getPluginId()const;

		virtual const size_t& getRTTI_hash() const override;


		Entity* getCamera( int index );
		EntityManager::EntityPointerContainer getCameras()const;

		void setDrawDistance( const unsigned int& drawDistance );
		unsigned int getDrawDistance()const;

		static const std::string PLUGIN_NAME;

		static const size_t type_hash;

		//configuration default values:
		static const std::string ANTIALIASING;//
		static const std::string FULLSCREEN;//
		static const std::string WINDOW_RESX;//only in windowed mode
		static const std::string WINDOW_RESY;//only in windowed mode
		static const std::string VSYNC;//
		static const std::string WINDOW_NAME;//
		static const std::string DRAW_DISTANCE;

		static const std::string ANTIALIASING_DEFAULT;
		static const std::string FULLSCREEN_DEFAULT;
		static const std::string WINDOW_RESX_DEFAULT;
		static const std::string WINDOW_RESY_DEFAULT;
		static const std::string VSYNC_DEFAULT;
		static const std::string WINDOW_NAME_DEFAULT;
		static const std::string DRAW_DISTANCE_DEFAULT;
	};
}
