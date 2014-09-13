#include "EntityManager.h"

namespace kg
{
	Entity::Id EntityManager::highestUniqueId = 0;

	Entity::Id EntityManager::getUniqueEntityId()
	{
		highestUniqueId++;
		return highestUniqueId --;
	}

	bool EntityManager::addEntity( std::shared_ptr<Entity>& entity )
	{
		auto id = entity->getId();

		auto it = m_entities.find( id );

		m_entities[id] = entity;

		//return false if Entity already existed
		return it == m_entities.end();
	}

	bool EntityManager::removeEntity( const Entity::Id& id )
	{
		bool didExist = m_entities.find( id )!=m_entities.end();
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

}
