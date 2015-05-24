#pragma once
#include "stdafx.h"
#include "id.h"
#include "Camera.h"

namespace kg
{

	//cameraStates, graphicsInformations
	//std::vector<std::shared_ptr<Entity>>
	typedef std::vector<std::shared_ptr<Entity>> CameraContainer;

	void drawingThreadFunction( sf::RenderWindow& renderWindow,
								boost::mutex& m_drawableEntityMutex,
								boost::mutex& cameraContainerMutex,
								CameraContainer& cameraContainer,
								EntityManager::EntityContainer& addedEntitiesCopy,
								EntityManager::EntityContainer& removedEntitiesCopy,
								int& drawingThreadFrameTime,
								bool& shouldTerminate,
								bool& drawingIsActive,
								boost::mutex& syncMutex,
								bool& threadHasToWait );

	class GraphicsSystem : public System, public CallbackReciever
	{
	private:

		std::shared_ptr<ConfigFile> m_configFile;
		struct ConfigValues
		{
			std::string* antialiasing;
			std::string* fullscreen;
			std::string* window_resx;
			std::string* window_resy;
			std::string* vsync;
			std::string* window_name;
		}m_configValues;

		mutable boost::mutex m_cameraContainerMutexA;
		mutable boost::mutex m_cameraContainerMutexB;
		CameraContainer m_cameras;
		bool m_shouldInitCameras = true;
		void m_initCameras( Engine& engine, World& world );

		mutable boost::mutex m_drawableEntityMutex;

		EntityManager::EntityContainer m_addedEntities;
		EntityManager::EntityContainer m_removedEntities;
		EntityManager::EntityContainer m_addedEntitiesCopy;
		EntityManager::EntityContainer m_removedEntitiesCopy;
		void m_onEntityAddedToWorld( const std::shared_ptr<Entity>& entity );
		void m_onEntityRemovedFromWorld( const std::shared_ptr<Entity>& entity );

		void m_onSavegameOpened( Engine& engine );
		void m_onSavegameClosed();

		void m_launchDrawingThread( sf::RenderWindow& renderWindow );
		void m_terminateDrawingThread();
		bool m_drawingShouldTerminate = false;
		bool m_drawingIsActive = false;
		int m_drawingThreadFrameTime = -1;

		mutable boost::mutex m_drawingThreadSyncMutex;
		bool m_drawingThreadHasToWait = true;

	public:
		GraphicsSystem();
		~GraphicsSystem();

		virtual void init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile );

		virtual void sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime );

		virtual double getUpdateImportance() const;

		virtual const Plugin::Name& getPluginName() const;

		virtual Plugin::Id getPluginId()const;

		virtual const size_t& getRTTI_hash() const override;


		std::shared_ptr<Entity> getCamera( int index );
		CameraContainer getCameras()const;

		static const std::string PLUGIN_NAME;

		static const size_t type_hash;

		//configuration default values:
		static const std::string ANTIALIASING;//
		static const std::string FULLSCREEN;//
		static const std::string WINDOW_RESX;//only in windowed mode
		static const std::string WINDOW_RESY;//only in windowed mode
		static const std::string VSYNC;//
		static const std::string WINDOW_NAME;//

		static const std::string ANTIALIASING_DEFAULT;
		static const std::string FULLSCREEN_DEFAULT;
		static const std::string WINDOW_RESX_DEFAULT;
		static const std::string WINDOW_RESY_DEFAULT;
		static const std::string VSYNC_DEFAULT;
		static const std::string WINDOW_NAME_DEFAULT;
	};
}
