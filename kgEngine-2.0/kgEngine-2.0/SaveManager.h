#pragma once
#include "stdafx.h"
#include "Entity.h"
#include "Callback.h"
#include "World.h"
#include "id_internal.h"
#include "SaveComponent.h"

namespace kg
{
	class DLL_EXPORT SaveManager
	{
		std::string m_openSavegameName;

	public:
		using SystemSaveInformationMap = std::map < int, SystemSaveInformation > ;

		std::vector<std::string> getAvailableSavegameNames()const;
		//loads systems
		void openSavegame( Engine& engine, World& world, const std::string& savegameName );
		const std::string& getOpenSavegameName()const;
		//filename without extension!
		bool loadEntitiesFromFile( Engine& engine, World& world, const std::string& filename );

		// filename without extension!
		// PREVIOUSLY STORED INFORMATION IN FILE(filename) WILL BE DELETED!
		void saveEntitiesToFile( const std::string& filename/*relative to open savegame path*/,
								 const std::vector<std::shared_ptr<Entity>>& entities );
		//saves every system that is registered in systemManager to globals.save
		void saveSystems( World& world );

	signals:
		Signal<Engine&> s_savegameOpened;
		Signal<> s_savegameClosed;
		std::map< Plugin::Id, boost::signals2::signal < std::vector<std::string>()> > s_writeSaveInformation;//first: systemId, second: signal
		std::map< Plugin::Id, Signal<const std::vector<std::string>&> > s_loadSaveInformation;//first: systemId, second: signal

		static const std::string PLUGIN_NAME;
		static const std::string SAVEGAME_FOLDER;
		static const std::string SYSTEM_SAVE_FILENAME;
		static const std::string SAVE_FILE_EXTENSION;
	};
}
