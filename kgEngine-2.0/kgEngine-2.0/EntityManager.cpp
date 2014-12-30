#include "EntityManager.h"

using namespace std;
using namespace sf;

namespace kg
{
	std::pair<bool, std::shared_ptr<Entity>> EntityManager::addEntity( std::shared_ptr<Entity>& entity )
	{
		Entity* id = entity.get();

		auto it = m_entities.find( id );
		auto end = m_entities.end();

		m_entities[id] = entity;

		s_entity_added( entity );

		//return false if Entity already existed
		return std::pair<bool, std::shared_ptr<Entity>>( it != end, entity );
	}

	bool EntityManager::removeEntity( const std::shared_ptr<Entity>& entity )
	{
		Entity* id = entity.get();

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

	std::shared_ptr<Entity> EntityManager::getEntity( const std::shared_ptr<Entity>& entity )
	{
		auto id = entity.get();

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
