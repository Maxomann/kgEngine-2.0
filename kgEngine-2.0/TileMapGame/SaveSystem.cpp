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
		return (int)id::SystemUpdateImportance::SAVE_SYSTEM;
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

	}

	const std::string& SavegameSystem::getOpenSavegameName() const
	{
		return m_openSavegame;
	}

	void SavegameSystem::saveEntitiesToFile( const std::string& fileName/*relative to open savegame path*/, const std::vector<std::shared_ptr<Entity>>& entities )
	{

	}

	void SavegameSystem::saveSystems()
	{

	}

	const std::string SavegameSystem::SAVEGAME_FOLDER ="./Saves";

	const std::string SavegameSystem::PLUGIN_NAME="SaveSystem";

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
		:m_blueprintEntityId(blueprintEntityId),
		m_uniqueEntityId( uniqueEntityId )
	{
		
	}

	int EntitySaveInformation::getActiveComponentId() const
	{
		return m_uniqueEntityId;
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

	}

	std::string EntitySaveInformation::toString() const
	{
		std::string retVal;
		retVal = to_string( m_blueprintEntityId );
		retVal += ";";
		retVal += to_string( m_uniqueEntityId );
		retVal += ";";
		for(const auto& el : m_information)
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
