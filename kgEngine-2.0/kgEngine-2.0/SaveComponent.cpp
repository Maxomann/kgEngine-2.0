#include "SaveComponent.h"
using namespace std;
using namespace sf;

namespace kg
{
	EntitySaveInformation::EntitySaveInformation( const std::string& constructFromString )
	{
		m_fromString( constructFromString );
	}

	EntitySaveInformation::EntitySaveInformation( unsigned int blueprintEntityId, Entity::Id uniqueEntityId )
		:m_blueprintEntityId( blueprintEntityId ),
		m_uniqueEntityId( uniqueEntityId )
	{ }

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

	unsigned int EntitySaveInformation::getBlueprintEntityId() const
	{
		return m_blueprintEntityId;
	}

	Entity::Id EntitySaveInformation::getUniqueEntityId() const
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

		stringstream( sblueprintId ) >> m_blueprintEntityId;
		stringstream( sUniqueId ) >> m_uniqueEntityId;
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
			if( el.second.size() > 0 )//check if there is information to save
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
		}

		return retVal;
	}

	SystemSaveInformation::SystemSaveInformation( const std::string& constructFromString )
	{
		m_fromString( constructFromString );
	}

	SystemSaveInformation::SystemSaveInformation( int systemPluginId )
		: m_systemPluginId( systemPluginId )
	{ }

	const std::vector<std::string> SystemSaveInformation::getInformation() const
	{
		return m_information;
	}

	void SystemSaveInformation::setInformation( const std::vector<std::string>& information )
	{
		m_information = information;
	}

	int SystemSaveInformation::getSystemPluginId() const
	{
		return m_systemPluginId;
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

		m_systemPluginId = atoi( sID.c_str() );
		boost::split( m_information, sValues, boost::is_any_of( ";" ) );
	}

	std::string SystemSaveInformation::toString() const
	{
		if( m_information.size() == 0 )
			return "";

		std::string retVal;
		retVal = std::to_string( m_systemPluginId );
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
		return m_information.size() != 0;
	}

	void Save::preInit( Engine& engine, const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues )
	{
		throw std::exception( "should not be called since Save is an internal Component and does not get initialized through this funtion" );
	}

	void Save::init( Engine& engine, World& world, ComponentManager& thisEntity )
	{
		return;
	}

	void Save::update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )
	{
		return;
	}

	double Save::getUpdateImportance() const
	{
		return id::SAVE_UPDATE_IMPORTANCE;
	}

	std::vector<Plugin::Id> Save::getRequieredComponents() const
	{
		return{ };
	}

	const std::string& Save::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id Save::getPluginId() const
	{
		return id::SAVE_COMPONENT_ID;
	}

	kg::EntitySaveInformation Save::writeSaveInformation() const
	{
		EntitySaveInformation retVal( m_blueprintEntityId, m_uniqueEntityId );
		for( const auto& el : s_writeSaveInformation )
		{
			retVal.setActiveComponentId( el.first );
			auto vec = el.second();
			if( vec.size() == 1 )
				retVal.addInformation( vec.at( 0 ) );
			else
				throw exception( "A save information callback in Save::writeSaveInformation had more or less than 1 object registered" );
		}

		return retVal;
	}

	void Save::loadSaveInformation( EntitySaveInformation& information )
	{
		for( auto& el : s_loadSaveInformation )
		{
			information.setActiveComponentId( el.first );
			el.second( information.getInformation() );
		}
	}

	Save::Save( const blueprint::Entity::Id& blueprintEntityId, const blueprint::Entity& blueprint, const Entity::Id& uniqueEntityId )
		:m_blueprintEntityId( blueprintEntityId ),
		m_uniqueEntityId( uniqueEntityId ),
		m_blueprint( blueprint )
	{ }

	const Entity::Id& Save::getUniqueId() const
	{
		return m_uniqueEntityId;
	}

	const blueprint::Entity::Id& Save::getBlueprintId() const
	{
		return m_blueprintEntityId;
	}

	const blueprint::Entity& Save::getBlueprint() const
	{
		return m_blueprint;
	}

	const size_t& Save::getRTTI_hash() const
	{
		return type_hash;
	}

	const std::string Save::PLUGIN_NAME = "internal_save";
}