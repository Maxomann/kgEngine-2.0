#include "Entity.h"
using namespace std;

namespace kg
{
	EntitySaveInformation::EntitySaveInformation( const std::string& constructFromString )
	{
		m_fromString( constructFromString );
	}

	EntitySaveInformation::EntitySaveInformation( unsigned int blueprintEntityId, Entity::Id uniqueEntityId )
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


	Entity::Entity( Engine& engine, const Id& id, const blueprint::Entity& entity )
		: m_id( id )
	{
		initFromBlueprint( engine, entity );
	}

	Entity::Entity( Engine& engine, const Id& id, const blueprint::Entity& entity, EntitySaveInformation& loadFrom )
		: m_id( id )
	{
		initFromBlueprint( engine, entity );
		loadSaveInformation( loadFrom );
	}

	Entity::Entity( const Id& id )
		:m_id(id)
	{

	}

	const Entity::Id& Entity::getId() const
	{
		return m_id;
	}

	void Entity::initFromBlueprint( Engine& engine,
									const blueprint::Entity& entity )//componentValuesByNameByComponent
	{
		//OLD ADDITIONAL BLUEPRINT VALUES CODE
		//m_additionalComponentValues = additionalBlueprintValues;//save additionalComponentValues for later request (ex. saving)

		auto components = entity.getComponentNames();
		for( const auto& name : components )
		{
			auto createdComponent = engine.pluginManager.createComponentPlugin( name );
			auto component = std::get<2>( createdComponent );

			auto componentValues = entity.getComponentValues( name );
			//OLD CODE: add additional component values to the map
			/*auto additionalComponentValuesForComponent = additionalBlueprintValues.find( name );
			if( additionalComponentValuesForComponent != additionalBlueprintValues.end() )
				for( const auto& el : (*additionalComponentValuesForComponent).second )
					componentValues[el.first] = el.second;//override existing value (if it exists)*/

			component->preInit( engine, componentValues );
			addComponent( component, std::get<1>( createdComponent ) );
		}


		// check component requirements of each component
		// throw exception if not all requirements are met
		for( const auto& component : getAllComponentsByTypeHash() )
		{
			for( const auto& requieredComponentHash : component.second->getRequieredComponents() )
			{
				if( !hasComponent( { requieredComponentHash } ) )
				{
					auto information = engine.pluginManager.getPluginInformationByRealTypeHashCode( requieredComponentHash );
					auto name = component.second->getPluginName();
					auto id = component.second->getPluginId();
					throw ComponentMissingException( name,
													 id,
													 information.second,
													 information.first );
				}
			}
		}

		initComponentsByImportance( engine );
	}

	void Entity::writeSaveInformation( EntitySaveInformation& writeTo )
	{
		for( const auto& el : getAllComponentsByPluginId() )
		{
			auto id = el.first;
			writeTo.setActiveComponentId( id );
			el.second->writeSaveInformation( writeTo );
		}
	}

	void Entity::loadSaveInformation( EntitySaveInformation& loadFrom )
	{
		for( const auto& el : getAllComponentsByPluginId() )
		{
			auto id = el.first;
			loadFrom.setActiveComponentId( id );
			el.second->loadSaveInformation( loadFrom );
		}
	}

}
