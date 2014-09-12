#include "EntityContainer.h"

namespace kg
{
	Entity::Id EntityContainer::highestUniqueId = 0;

	Entity::Id EntityContainer::getUniqueEntityId()
	{
		highestUniqueId++;
		return highestUniqueId --;
	}

	bool EntityContainer::addEntity( std::shared_ptr<Entity>& entity )
	{
		auto id = entity->getId();

		auto it = m_entities.find( id );

		m_entities[id] = entity;

		//return false if Entity already existed
		return it == m_entities.end();
	}

	bool EntityContainer::removeEntity( const Entity::Id& id )
	{
		bool didExist = m_entities.find( id )!=m_entities.end();
		m_entities.erase( id );

		return didExist;
	}

	std::shared_ptr<Entity> EntityContainer::getEntity( const Entity::Id& id )
	{
		auto it = m_entities.find( id );
		if( it == m_entities.end() )
			return nullptr;
		else
			return it->second;
	}

	void EntityContainer::updateEntities( Engine& engine, World& world )
	{
		for( auto& entity : m_entities )
			entity.second->updateAllComponentsByImportance( engine, world );
	}

}
