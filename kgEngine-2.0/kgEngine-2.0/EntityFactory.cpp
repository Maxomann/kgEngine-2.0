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

	Entity EntityFactory::createNewSaveableEntity( Engine& engine,
												   World& world,
												   const int& entityBlueprintId )
	{
		auto temp = createNewSaveableEntity( engine, world, entityBlueprintId, getUniqueEntityId() );
		return temp;
	}

	Entity EntityFactory::createNewSaveableEntity( Engine& engine,
												   World& world,
												   const int& entityBlueprintId,
												   const Entity::Id& uniqueId )
	{
		Entity entity;

		auto& entityBlueprint = engine.blueprint.getEntityById( entityBlueprintId );

		auto& components = entityBlueprint.getComponentsByName();
		for( const auto& comp : components )
		{
			auto component = engine.pluginManager.createPlugin<Component>( comp.first );

			component->preInit( engine, comp.second.getComponentValueReferencesByName() );
			entity.addComponent( move( component ) );
		}

		unique_ptr<Component> saveComponent = std::make_unique<Save>( entityBlueprintId, entityBlueprint, uniqueId );
		entity.addComponent( saveComponent );

		// check component requirements of each component
		// throw exception if not all requirements are met
		for( const auto& component : entity.getAllComponentsByUpdateImportance() )
		{
			for( const auto& requieredComponentId : component->getRequieredComponents() )
			{
				if( !entity.hasComponent( requieredComponentId ) )
				{
					throw ComponentMissingException( component->getPluginId(),
													 requieredComponentId );
				}
			}
		}

		entity.initComponentsByImportance( engine, world );

		return move( entity );
	}
}