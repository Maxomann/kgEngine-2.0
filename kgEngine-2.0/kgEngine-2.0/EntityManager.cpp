#include "EntityManager.h"

using namespace std;
using namespace sf;

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

		s_entity_added( entity );

		//return false if Entity already existed
		return std::pair<bool, std::shared_ptr<Entity>>( it != end, entity );
	}

	bool EntityManager::removeEntity( const Entity::Id& id )
	{
		auto it = m_entities.find( id );

		bool didExist = false;

		if( it != m_entities.end() )
		{
			s_entity_removed( it->second );
			didExist = true;
			m_entities.erase( id );
		}

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

	void EntityManager::updateEntities( Engine& engine, World& world, const sf::Time& frameTime )
	{
		for( auto& entity : m_entities )
			entity.second->updateAllComponentsByImportance( engine, world, frameTime );
	}

	std::shared_ptr<Entity> EntityManager::createNewEntity( Engine& engine,
														 const int& entityBlueprintId )
	{
		return createNewEntity( engine, entityBlueprintId, getUniqueEntityId() );
	}

	std::shared_ptr<Entity> EntityManager::createNewEntity( Engine& engine,
														 const int& entityBlueprintId,
														 const Entity::Id& uniqueId )
	{
		auto returnValue = std::make_shared<Entity>( engine,
													 uniqueId,
													 engine.blueprint.getEntityById( entityBlueprintId ) );

		return returnValue;
	}

	void EntityManager::setLowestUniqueEntityId( const Entity::Id& id )
	{
		m_highestUniqueId = id;
	}

	std::shared_ptr<Entity> EntityManager::loadEntity( Engine& engine, EntitySaveInformation& saveInformation )
	{
		auto returnValue = std::make_shared<Entity>( engine,
													 saveInformation.getUniqueEntityId(),
													 engine.blueprint.getEntityById( saveInformation.getBlueprintEntityId() ),
													 saveInformation );

		return returnValue;
	}

	void EntityManager::clear()
	{
		m_entities.clear();
	}

	std::vector<std::shared_ptr<Entity>> EntityManager::getAllEntities()
	{
		vector<shared_ptr<Entity>> retVal;
		for( auto& el : m_entities )
			retVal.push_back( el.second );
		return retVal;
	}

}
