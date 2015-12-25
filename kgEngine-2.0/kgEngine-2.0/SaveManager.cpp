#include "SaveManager.h"
using namespace std;
using namespace sf;
using namespace tr2;
using namespace sys;

namespace kg
{
	std::vector<std::string> SaveManager::getAvailableSavegameNames() const
	{
		std::vector<std::string> retVal;

		for( auto saveIt = directory_iterator( SAVEGAME_FOLDER ); saveIt != directory_iterator(); ++saveIt )
		{
			path currentPath = *saveIt;
			if( is_directory( currentPath ) )
				retVal.push_back( currentPath.filename().string() );
		}

		return retVal;
	}

	void SaveManager::openSavegame( Engine& engine, World& world, const std::string& savegameName )
	{
		closeSavegame( engine, world );

		m_openSavegameName = savegameName;

		//create savegame folder if it does not exist
		create_directories( path( SAVEGAME_FOLDER + "/" + savegameName ) );

		fstream globalsFile( SAVEGAME_FOLDER + "/" + savegameName + "/" + SYSTEM_SAVE_FILENAME + SAVE_FILE_EXTENSION,
							 ios::in );
		vector<string> lines;
		string line;
		while( getline( globalsFile, line ) )
			lines.push_back( std::move( line ) );

		//load lowest unique entityId
		Entity::Id lowestUniqueId = 0;
		if( lines.size() > 0 )
		{
			stringstream( lines.at( 0 ) ) >> lowestUniqueId;
			lines.erase( lines.begin() );//remove that element
		}
		world.setLowestUniqueEntityId( lowestUniqueId );

		for( const auto& el : lines )
		{
			if( el != "" )
			{
				SystemSaveInformation info( el );
				s_loadSaveInformation[info.getSystemPluginId()]( info.getInformation() );
			}
		}
		globalsFile.close();

		s_savegameOpened( engine, world );//inform registered systems (even if no save information has been loaded for them!)

		return;
	}

	void SaveManager::closeSavegame( Engine& engine, World& world )
	{
		world.clear();
		s_savegameClosed();
	}

	const std::string& SaveManager::getOpenSavegameName() const
	{
		return m_openSavegameName;
	}

	void SaveManager::saveSystems( World& world )
	{
		std::vector<SystemSaveInformation> saveInformationVec;
		for( const auto& el : s_writeSaveInformation )
		{
			SystemSaveInformation info( el.first );
			auto vec = el.second();
			if( vec.size() == 1 )
			{
				info.setInformation( vec.at( 0 ) );
				saveInformationVec.push_back( info );
			}
			else
				throw exception( "A save information callback in SaveManager::saveSystems had more or less than 1 object registered" );
		}

		fstream globalsFile( SAVEGAME_FOLDER + "/" + m_openSavegameName + "/" + SYSTEM_SAVE_FILENAME + SAVE_FILE_EXTENSION,
							 fstream::out | fstream::trunc );
		globalsFile << to_string( world.getUniqueEntityId() ) << endl;//save lowest unique entity id
		for( const auto& el : saveInformationVec )
		{
			auto data = el.toString();
			if( data != "" )
				globalsFile << el.toString() << endl;
		}

		globalsFile.close();
		return;
	}

	boost::optional<std::vector<Entity>> SaveManager::loadEntitiesFromFile( Engine& engine, World& world, const std::string& filename )
	{
		//return false if file does not exist
		if( !exists( path( string( SAVEGAME_FOLDER + "/" + m_openSavegameName + "/" + filename + SAVE_FILE_EXTENSION ) ) ) )
			return;

		fstream file( SAVEGAME_FOLDER + "/" + m_openSavegameName + "/" + filename + SAVE_FILE_EXTENSION,
					  fstream::in );
		if( !file.is_open() )
			return;

		vector<string> lines;
		string line;
		while( getline( file, line ) )
			lines.push_back( std::move( line ) );

		vector<EntitySaveInformation> information;
		for( const auto& el : lines )
			if( el != "" )
				information.push_back( EntitySaveInformation( el ) );

		vector<Entity> entities;

		for( auto& el : information )
		{
			auto entity = world.createNewSaveableEntity( engine, world, el.getBlueprintEntityId(), el.getUniqueEntityId() );
			entity.getComponent<Save>()->loadSaveInformation( el );
			entities.push_back( move( entity ) );
		}

		file.close();

		return entities;
	}

	void SaveManager::saveEntitiesToFile( const std::string& path,
										  const World::EntityPointerContainer& entities )
	{
		fstream file( SAVEGAME_FOLDER + "/" + m_openSavegameName + "/" + path + SAVE_FILE_EXTENSION,
					  fstream::out | fstream::trunc );

		for( const auto& el : entities )
		{
			auto saveComponent = el->getComponentTry<Save>();
			if( saveComponent )//can only be saved if it has a save component
			{
				auto saveInformation = saveComponent->writeSaveInformation();
				file << saveInformation.toString() << endl;
			}
		}

		file.close();
		return;
	}

	const std::string SaveManager::SAVE_FILE_EXTENSION = ".save";

	const std::string SaveManager::SYSTEM_SAVE_FILENAME = "_global";

	const std::string SaveManager::SAVEGAME_FOLDER = "./Saves";

	const std::string SaveManager::PLUGIN_NAME = "SaveSystem";
}