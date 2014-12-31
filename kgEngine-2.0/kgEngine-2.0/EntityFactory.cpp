#include "EntityFactory.h"
using namespace std;
using namespace sf;

namespace kg
{
	Entity::Id EntityFactory::getUniqueEntityId()
	{
		m_highestUniqueId++;
		return m_highestUniqueId - 1;
	}

	void EntityFactory::setLowestUniqueEntityId( const Entity::Id& id )
	{
		m_highestUniqueId = id;
	}

	std::shared_ptr<Entity> EntityFactory::createNewSaveableEntity( Engine& engine,
															const int& entityBlueprintId )
	{
		return createNewSaveableEntity( engine, entityBlueprintId, getUniqueEntityId() );
	}

	std::shared_ptr<Entity> EntityFactory::createNewSaveableEntity( Engine& engine,
															const int& entityBlueprintId,
															const Entity::Id& uniqueId )
	{
		auto entity = std::make_shared<Entity>();

		auto entityBlueprint = engine.blueprint.getEntityById( entityBlueprintId );

		//OLD ADDITIONAL BLUEPRINT VALUES CODE
		//m_additionalComponentValues = additionalBlueprintValues;//save additionalComponentValues for later request (ex. saving)
		auto components = entityBlueprint.getComponentNames();
		for( const auto& name : components )
		{
			auto createdComponent = engine.pluginManager.createComponentPlugin( name );
			auto component = std::get<2>( createdComponent );

			auto componentValues = entityBlueprint.getComponentValues( name );
			//OLD CODE: add additional component values to the map
			/*auto additionalComponentValuesForComponent = additionalBlueprintValues.find( name );
			if( additionalComponentValuesForComponent != additionalBlueprintValues.end() )
			for( const auto& el : (*additionalComponentValuesForComponent).second )
			componentValues[el.first] = el.second;//override existing value (if it exists)*/

			component->preInit( engine, componentValues );
			entity->addComponent( component, std::get<1>( createdComponent ) );
		}

		entity->addComponent<Save>( std::static_pointer_cast< Component >(std::make_shared<Save>( entityBlueprintId, uniqueId )) );

		// check component requirements of each component
		// throw exception if not all requirements are met
		for( const auto& component : entity->getAllComponentsByTypeHash() )
		{
			for( const auto& requieredComponentHash : component.second->getRequieredComponents() )
			{
				if( !entity->hasComponent( { requieredComponentHash } ) )
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

		entity->initComponentsByImportance( engine );

		return entity;
	}
}