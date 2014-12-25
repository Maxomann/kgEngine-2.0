//#include "SaveSystem.h"
//using namespace std;
//using namespace sf;
//using namespace tr2;
//using namespace sys;
//
//namespace kg
//{
//	void SavegameSystem::init( Engine& engine, World& world, std::shared_ptr<ConfigFile>& configFile )
//	{
//		return;
//	}
//
//	void SavegameSystem::sfmlEvent( Engine& engine, const sf::Event& sfEvent )
//	{
//		return;
//	}
//
//	void SavegameSystem::update( Engine& engine, World& world, const sf::Time& frameTime )
//	{
//		return;
//	}
//
//	double SavegameSystem::getUpdateImportance() const
//	{
//		return ( int )id::SystemUpdateImportance::SAVE_SYSTEM;
//	}
//
//	const std::string& SavegameSystem::getPluginName() const
//	{
//		return PLUGIN_NAME;
//	}
//
//	int SavegameSystem::getPluginId() const
//	{
//		return ( int )id::SystemPluginId::SAVE_SYSTEM;
//	}
//
//	std::vector<std::string> SavegameSystem::getAvailableSavegameNames() const
//	{
//		std::vector<std::string> retVal;
//
//		for( auto saveIt = directory_iterator( SAVEGAME_FOLDER ); saveIt != directory_iterator(); ++saveIt )
//		{
//			path currentPath = *saveIt;
//			if( is_directory( currentPath ) )
//				retVal.push_back( currentPath.filename() );
//		}
//
//		return retVal;
//	}
//
//	void SavegameSystem::openSavegame( Engine& engine, World& world, const std::string& savegameName )
//	{
//		m_openSavegameName = savegameName;
//
//		//create savegame folder if it does not exist
//		create_directories( path( SAVEGAME_FOLDER + "/" + savegameName ) );
//
//		fstream globalsFile( SAVEGAME_FOLDER + "/" + savegameName + SYSTEM_SAVE_FILENAME + SAVE_FILE_EXTENSION,
//							 ios::in );
//		vector<string> lines;
//		string line;
//		while( getline( globalsFile, line ) )
//			lines.push_back( std::move( line ) );
//
//		//load lowest unique entityId
//		Entity::Id lowestUniqueId=0;
//		if( lines.size() > 0 )
//		{
//			stringstream( lines.at( 0 ) ) >> lowestUniqueId;
//			lines.erase( lines.begin() );//remove that element
//		}
//		world.setLowestUniqueEntityId( lowestUniqueId );
//
//
//		SystemSaveInformationMap systemSaveInformationMap;
//
//		for( const auto& el : lines )
//		{
//			SystemSaveInformation info( el );
//			systemSaveInformationMap.insert( std::pair<int, SystemSaveInformation>( info.getSystemId(), info ) );
//		}
//
//		s_onSystemLoad( systemSaveInformationMap );//notify systems
//	}
//
//	const std::string& SavegameSystem::getOpenSavegameName() const
//	{
//		return m_openSavegameName;
//	}
//
//	void SavegameSystem::saveSystems( World& world )
//	{
//		auto saveInformationVec = s_onSystemSave();
//		fstream globalsFile( SAVEGAME_FOLDER + "/" + m_openSavegameName + SYSTEM_SAVE_FILENAME + SAVE_FILE_EXTENSION,
//							 fstream::out | fstream::trunc );
//		globalsFile << to_string( world.getUniqueEntityId() ) << endl;//save lowest unique entity id
//		for( const auto& el : saveInformationVec )
//			globalsFile << el.toString() << endl;
//
//		return;
//	}
//
//	void SavegameSystem::loadEntitiesFromFile( Engine& engine, World& world, const std::string& filename )
//	{
//		fstream file( SAVEGAME_FOLDER + "/" + m_openSavegameName + filename + SAVE_FILE_EXTENSION,
//					  fstream::in );
//
//		vector<string> lines;
//		string line;
//		while( getline( file, line ) )
//			lines.push_back( std::move( line ) );
//
//		vector<EntitySaveInformation> information;
//		for( const auto& el : lines )
//			information.push_back( EntitySaveInformation( el ) );
//
//		for( const auto& el : information )
//		{
//			auto entity = world.createEntity( engine, el.getBlueprintEntityId(), el.getUniqueEntityId() );
//			//pass save information to entity
//			//entity.getComponent<Save>()->onLoad(el);
//		}
//	}
//
//	void SavegameSystem::saveEntitiesToFile( const std::string& fileName/*relative to open savegame path*/, const std::vector<std::shared_ptr<Entity>>& entities )
//	{
//
//	}
//
//	const std::string SavegameSystem::SAVE_FILE_EXTENSION = ".save";
//
//	const std::string SavegameSystem::SYSTEM_SAVE_FILENAME = "_global.save";
//
//	const std::string SavegameSystem::SAVEGAME_FOLDER = "./Saves";
//
//	const std::string SavegameSystem::PLUGIN_NAME = "SaveSystem";
//
//}
