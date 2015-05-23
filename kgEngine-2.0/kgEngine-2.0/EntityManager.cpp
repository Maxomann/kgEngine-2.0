#include "EntityManager.h"

using namespace std;
using namespace sf;

namespace kg
{
	void EntityManager::addEntity( std::shared_ptr<Entity>& entity )
	{
		m_entities.push_back( entity );
		s_entity_added( entity );
	}

	void EntityManager::removeEntity( const std::shared_ptr<Entity>& entity )
	{
		s_entity_removed( entity );
		auto it = m_findEntity( entity );
		if( it != m_entities.end() )
			m_entities.erase( it );
	}

	bool EntityManager::doesEntityExist( const std::shared_ptr<Entity>& entity )
	{
		return m_findEntity( entity ) != m_entities.end();
	}

	void EntityManager::updateEntities( Engine& engine, World& world, const sf::Time& frameTime )
	{
		for( auto& entity : m_entities )
			entity->updateAllComponentsByImportance( engine, world, frameTime );
	}

	void EntityManager::clear()
	{
		m_entities.clear();
	}

	const EntityManager::EntityContainer& EntityManager::getAllEntities() const
	{
		return m_entities;
	}

	unsigned int EntityManager::getEntityCount() const
	{
		return m_entities.size();
	}

	EntityManager::EntityContainer::iterator EntityManager::m_findEntity( const std::shared_ptr<Entity>& entity )
	{
		return std::find( begin( m_entities ), end( m_entities ), entity );
	}
}
