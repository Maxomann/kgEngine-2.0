#include "SaveSystem.h"
using namespace std;
using namespace sf;
using namespace tr2;
using namespace sys;

namespace kg
{
	void SavegameSystem::init( Engine& engine, World& world, std::shared_ptr<ConfigFile>& configFile )
	{
		return;
	}

	void SavegameSystem::sfmlEvent( Engine& engine, const sf::Event& sfEvent )
	{
		return;
	}

	void SavegameSystem::update( Engine& engine, World& world, const sf::Time& frameTime )
	{
		return;
	}

	double SavegameSystem::getUpdateImportance() const
	{
		return ( int )id::SystemUpdateImportance::SAVE_SYSTEM;
	}

	const std::string& SavegameSystem::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	int SavegameSystem::getPluginId() const
	{
		return ( int )id::SystemPluginId::SAVE_SYSTEM;
	}

	std::vector<std::string> SavegameSystem::getAvailableSavegameNames() const
	{
		std::vector<std::string> retVal;

		for( auto saveIt = directory_iterator( SAVEGAME_FOLDER ); saveIt != directory_iterator(); ++saveIt )
		{
			path currentPath = *saveIt;
			if( is_directory( currentPath ) )
				retVal.push_back( currentPath.filename() );
		}

		return retVal;
	}

	void SavegameSystem::openSavegame( Engine& engine, World& world, const std::string& savegameName )
	{
		m_openSavegameName = savegameName;

		//create savegame folder if it does not exist
		create_directories( path(SAVEGAME_FOLDER + "/" + savegameName) );

		fstream globalsFile( SAVEGAME_FOLDER + "/" + savegameName + SYSTEM_SAVE_FILENAME + SAVE_FILE_EXTENSION,
							 ios::in );
		vector<string> lines;
		string line;
		while( getline( globalsFile, line ) )
			lines.push_back( std::move(line) );

		SystemSaveInformationMap systemSaveInformationMap;

		for( const auto& el : lines )
		{
			SystemSaveInformation info( el );
			systemSaveInformationMap.insert(std::pair<int, SystemSaveInformation>(info.getSystemId(), info));
		}

		s_onSystemLoad( systemSaveInformationMap );//notify systems
	}

	const std::string& SavegameSystem::getOpenSavegameName() const
	{
		return m_openSavegameName;
	}

	void SavegameSystem::saveSystems()
	{
		auto saveInformationVec = s_onSystemSave();
		fstream globalsFile( SAVEGAME_FOLDER + "/" + m_openSavegameName + SYSTEM_SAVE_FILENAME + SAVE_FILE_EXTENSION,
							 fstream::out | fstream::trunc );
		for( const auto& el : saveInformationVec )
			globalsFile << el.toString()<< endl;

		return;
	}

	const std::string SavegameSystem::SAVE_FILE_EXTENSION = ".save";

	const std::string SavegameSystem::SYSTEM_SAVE_FILENAME = "_global.save";

	const std::string SavegameSystem::SAVEGAME_FOLDER = "./Saves";

	const std::string SavegameSystem::PLUGIN_NAME = "SaveSystem";

	SystemSaveInformation::SystemSaveInformation( const std::string& constructFromString )
	{
		m_fromString( constructFromString );
	}

	SystemSaveInformation::SystemSaveInformation( int systemId )
		: m_systemId( systemId )
	{

	}

	const std::vector<std::string> SystemSaveInformation::getInformation() const
	{
		return m_information;
	}

	void SystemSaveInformation::setInformation( const std::vector<std::string>& information )
	{
		m_information = information;
	}

	int SystemSaveInformation::getSystemId() const
	{
		return m_systemId;
	}

	void SystemSaveInformation::m_fromString( const std::string& str )
	{
		m_information.clear();

		std::string sID;
		std::string sValues;
		bool isInValues = false;
		for( const auto& el : str )
		{
			if( isInValues )
			{
				sValues.push_back( el );
			}
			else
			{
				if( el == '[' )
					isInValues = true;
				else
					sID.push_back( el );
			}
		}
		sValues.pop_back();//because last char is ']'

		m_systemId = atoi( sID.c_str() );
		boost::split( m_information, sValues, boost::is_any_of( ";" ) );
	}

	std::string SystemSaveInformation::toString() const
	{
		if( m_information.size() == 0 )
			return "";

		std::string retVal;
		retVal = std::to_string( m_systemId );
		retVal += "[";
		for( const auto & el : m_information )
		{
			retVal += el;
			retVal += ";";
		}
		retVal.pop_back();
		retVal += "]";

		return retVal;
	}

	bool SystemSaveInformation::hasInformation() const
	{
		return m_information.size();
	}


	EntitySaveInformation::EntitySaveInformation( const std::string& constructFromString )
	{
		m_fromString( constructFromString );
	}

	EntitySaveInformation::EntitySaveInformation( int blueprintEntityId, long long int uniqueEntityId )
		:m_blueprintEntityId( blueprintEntityId ),
		m_uniqueEntityId( uniqueEntityId )
	{

	}

	int EntitySaveInformation::getActiveComponentId() const
	{
		return m_activeComponentId;
	}

	void EntitySaveInformation::setActiveComponentId( int id )
	{
		m_activeComponentId = id;
		m_information[id];//to prevent errors in getInformation
	}

	const std::vector<std::string>& EntitySaveInformation::getInformation() const
	{
		return m_information.at( m_activeComponentId );
	}

	void EntitySaveInformation::addInformation( const std::vector<std::string>& information )
	{
		m_information.at( m_activeComponentId ) = information;
	}

	int EntitySaveInformation::getBlueprintEntityId() const
	{
		return m_blueprintEntityId;
	}

	long long int EntitySaveInformation::getUniqueEntityId() const
	{
		return m_uniqueEntityId;
	}

	void EntitySaveInformation::m_fromString( const std::string& str )
	{
		m_information.clear();

		string sblueprintId;
		string sUniqueId;
		map<int, string> sValuesByComponentId;
		bool first = false;
		bool second = false;

		std::string rawValueString;

		for( const auto& el : str )
		{
			if( first )
			{
				if( second )
				{
					//second
					rawValueString.push_back( el );
				}
				else
				{
					//only first
					if( el == ';' )
						second = true;
					else
						sUniqueId.push_back( el );
				}
			}
			else
			{
				//none
				if( el == ';' )
					first = true;
				else
					sblueprintId.push_back( el );
			}
		}

		vector<string> splitComponents;
		rawValueString.pop_back();
		boost::split( splitComponents, rawValueString, boost::is_any_of( "]" ) );
		for( const auto& el : splitComponents )
		{
			vector<string> secondSplit;//0:componentId 1:values
			boost::split( secondSplit, el, boost::is_any_of( "[" ) );

			if( secondSplit.size() != 2 )
				throw exception();

			int componentId = atoi( secondSplit.at( 0 ).c_str() );
			boost::split( m_information[componentId], secondSplit.at( 1 ), boost::is_any_of( ";" ) );
		}

		m_blueprintEntityId = atoi( sblueprintId.c_str() );
		m_uniqueEntityId = atoll( sUniqueId.c_str() );

	}

	std::string EntitySaveInformation::toString() const
	{
		std::string retVal;
		retVal = to_string( m_blueprintEntityId );
		retVal += ";";
		retVal += to_string( m_uniqueEntityId );
		retVal += ";";
		for( const auto& el : m_information )
		{
			retVal += to_string( el.first );//componentId
			retVal += "[";
			for( const auto& val : el.second )
			{
				retVal += val;
				retVal += ";";
			}
			retVal.pop_back();
			retVal += "]";
		}

		return retVal;
	}

}
