#pragma once
#include "stdafx.h"

namespace kg
{
	class SavegameSystem : public System
	{
		std::string m_openSavegameName;

	public:
		using SystemSaveInformationMap = std::map < int, SystemSaveInformation > ;

		virtual void init( Engine& engine, World& world, std::shared_ptr<ConfigFile>& configFile );

		virtual void sfmlEvent( Engine& engine, const sf::Event& sfEvent );

		virtual void update( Engine& engine, World& world, const sf::Time& frameTime );

		virtual double getUpdateImportance() const;

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;

		virtual void writeSaveInformation( SystemSaveInformation& writeTo ) override;

		virtual void loadSaveInformation( const SystemSaveInformation& loadFrom ) override;



		std::vector<std::string> getAvailableSavegameNames()const;
		//loads systems
		void openSavegame( Engine& engine, World& world, const std::string& savegameName );
		const std::string& getOpenSavegameName()const;
		//filename without extension!
		void loadEntitiesFromFile( Engine& engine, World& world, const std::string& filename );//TODO

		// filename without extension!
		// PREVIOUSLY STORED INFORMATION IN FILE(filename) WILL BE DELETED!
		void saveEntitiesToFile( const std::string& filename/*relative to open savegame path*/,
								 const std::vector<std::shared_ptr<Entity>>& entities );//TODO
		//saves every system that is registered in systemManager to globals.save
		void saveSystems( World& world );

	signals:
		Signal<Engine&, World&> s_savegameOpened;

		static const std::string PLUGIN_NAME;
		static const std::string SAVEGAME_FOLDER;
		static const std::string SYSTEM_SAVE_FILENAME;
		static const std::string SAVE_FILE_EXTENSION;
	};
}
