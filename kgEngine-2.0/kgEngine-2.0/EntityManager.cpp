#include "EntityManager.h"

namespace kg
{
	Entity::Id EntityManager::getUniqueEntityId()
	{
		m_highestUniqueId++;
		return m_highestUniqueId - 1;
	}

	std::pair<bool, std::shared_ptr<Entity>> EntityManager::addEntity( std::shared_ptr<Entity>& entity )
	{
		auto id = entity->getId();

		auto it = m_entities.find( id );
		auto end = m_entities.end();

		m_entities[id] = entity;

		//return false if Entity already existed
		return std::pair<bool, std::shared_ptr<Entity>>( it != end, entity );
	}

	bool EntityManager::removeEntity( const Entity::Id& id )
	{
		bool didExist = m_entities.find( id ) != m_entities.end();
		m_entities.erase( id );

		return didExist;
	}

	std::shared_ptr<Entity> EntityManager::getEntity( const Entity::Id& id )
	{
		auto it = m_entities.find( id );
		if( it == m_entities.end() )
			return nullptr;
		else
			return it->second;
	}

	void EntityManager::updateEntities( Engine& engine, World& world )
	{
		for( auto& entity : m_entities )
			entity.second->updateAllComponentsByImportance( engine, world );
	}

	std::shared_ptr<Entity> EntityManager::createEntity( Engine& engine,
														 const int& entityBlueprintId,
														 const blueprint::ComponentValuesByNameByComponentMap& additionalBlueprintValues )
	{
		auto returnValue = std::make_shared<Entity>( getUniqueEntityId() );
		returnValue->initFromBlueprint( engine, engine.blueprint.getEntityById( entityBlueprintId ), additionalBlueprintValues );

		return returnValue;
	}

	void EntityManager::setLowestUniqueEntityId( const Entity::Id& id )
	{
		m_highestUniqueId = id;
	}
}