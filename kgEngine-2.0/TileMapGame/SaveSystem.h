#pragma once
#include "stdafx.h"

namespace kg
{
	class SystemSaveInformation
	{
		int m_systemId;

		std::vector<std::string> m_information;

		void m_fromString( const std::string& str );

	public:
		SystemSaveInformation( const std::string& constructFromString );
		SystemSaveInformation( int systemId );

		const std::vector<std::string> getInformation()const;
		void setInformation( const std::vector<std::string>& information );
		bool hasInformation()const;

		int getSystemId()const;

		std::string toString()const;

	};
	class EntitySaveInformation
	{
		using InformationByComponentIdMap = std::map < int, std::vector<std::string> >;

		int m_blueprintEntityId;
		long long int m_uniqueEntityId;

		InformationByComponentIdMap m_information;

		int m_activeComponentId = -1;

		void m_fromString( const std::string& str );

	public:
		EntitySaveInformation( const std::string& constructFromString );
		EntitySaveInformation( int blueprintEntityId, long long int uniqueEntityId );

		int getActiveComponentId()const;
		void setActiveComponentId( int id );

		const std::vector<std::string>& getInformation()const;

		//for active componentId
		void addInformation( const std::vector<std::string>& information );

		int getBlueprintEntityId()const;
		long long int getUniqueEntityId()const;

		std::string toString()const;
	};



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


		std::vector<std::string> getAvailableSavegameNames()const;
		//loads systems
		void openSavegame( Engine& engine, World& world, const std::string& savegameName );
		const std::string& getOpenSavegameName()const;
		void loadEntitiesFromFile( Engine& engine, World& world, const std::string& filename );//TODO

		void saveEntitiesToFile( const std::string& fileName/*relative to open savegame path*/,
								 const std::vector<std::shared_ptr<Entity>>& entities );//TODO
		//saves every system that is registered in systemManager to globals.save
		void saveSystems();


	signals:
		boost::signals2::signal<SystemSaveInformation(),
			aggregate_values<std::vector<SystemSaveInformation>>> s_onSystemSave;
		Signal<const SystemSaveInformationMap&> s_onSystemLoad;

		static const std::string PLUGIN_NAME;
		static const std::string SAVEGAME_FOLDER;
		static const std::string SYSTEM_SAVE_FILENAME;
		static const std::string SAVE_FILE_EXTENSION;
	};
}
