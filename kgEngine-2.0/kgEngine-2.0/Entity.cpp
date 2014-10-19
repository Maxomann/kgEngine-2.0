#include "Entity.h"

namespace kg
{
	Entity::Entity( const Id& id )
		:m_id( id )
	{ }

	const Entity::Id& Entity::getId() const
	{
		return m_id;
	}

	void Entity::initFromBlueprint( Engine& engine,
									const blueprint::Entity& entity,
									const blueprint::ComponentValuesByNameByComponentMap& additionalBlueprintValues )//componentValuesByNameByComponent
	{
		m_additionalComponentValues = additionalBlueprintValues;//save additionalComponentValues for later request (ex. saving)

		auto components = entity.getComponentNames();
		for( const auto& name : components )
		{
			auto createdComponent = engine.pluginManager.createComponentPlugin( name );
			auto component = std::get<2>( createdComponent );

			auto componentValues = entity.getComponentValues( name );
			//add additional component values to the map
			auto additionalComponentValues = additionalBlueprintValues.find( name );
			if( additionalComponentValues != additionalBlueprintValues.end() )
				for( const auto& el : (*additionalComponentValues).second )
					componentValues[el.first] = el.second;

			component->preInit( componentValues );
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
					auto information = engine.pluginManager.getPlugininformationByRealTypeHashCode( requieredComponentHash );
					throw ComponentMissingException( component.second->getPluginName(),
													 component.second->getPluginId(),
													 information.second,
													 information.first );
				}
			}
		}

		initComponentsByImportance( engine );
	}

	const blueprint::ComponentValuesByNameByComponentMap& Entity::getAdditionalComponentValues() const
	{
		return m_additionalComponentValues;
	}

}
