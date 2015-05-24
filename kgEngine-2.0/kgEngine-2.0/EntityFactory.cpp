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
																	World& world,
																	const int& entityBlueprintId )
	{
		return createNewSaveableEntity( engine, world, entityBlueprintId, getUniqueEntityId() );
	}

	std::shared_ptr<Entity> EntityFactory::createNewSaveableEntity( Engine& engine,
																	World& world,
																	const int& entityBlueprintId,
																	const Entity::Id& uniqueId )
	{
		auto entity = std::make_shared<Entity>();

		auto& entityBlueprint = engine.blueprint.getEntityById( entityBlueprintId );

		auto& components = entityBlueprint.getComponentsByName();
		for( const auto& comp : components )
		{
			auto component = engine.pluginManager.createPlugin<Component>( comp.first );

			component->preInit( engine, comp.second.getComponentValueReferencesByName() );
			entity->addComponent( component );
		}

		entity->addComponent(
			std::static_pointer_cast< Component >(
			std::make_shared<Save>( entityBlueprintId, entityBlueprint, uniqueId )
			) );

		// check component requirements of each component
		// throw exception if not all requirements are met
		for( const auto& component : entity->getAllComponents() )
		{
			for( const auto& requieredComponentId : component->getRequieredComponents() )
			{
				if( !entity->hasComponent( requieredComponentId ) )
				{
					throw ComponentMissingException( component->getPluginId(),
													 requieredComponentId );
				}
			}
		}

		entity->initComponentsByImportance( engine, world );

		return entity;
	}
}
