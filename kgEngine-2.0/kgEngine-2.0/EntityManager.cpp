#include "EntityManager.h"

using namespace std;
using namespace sf;

namespace kg
{

	EntityManager::EntityManager()
	{
		m_entities.reserve( EXPECTED_MAX_ENTITY_COUNT );
	}

	void EntityManager::addEntity( std::shared_ptr<Entity>& entity )
	{
		m_entities.push_back( entity );
		s_entity_added( entity );
	}

	void EntityManager::removeEntity( const std::shared_ptr<Entity>& entity )
	{
		s_entity_removed( entity );
		m_toRemove.push_back( entity );
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

	void EntityManager::removeEntitiesOnRemoveList()
	{
		m_entities.erase( std::remove_if( m_entities.begin(), m_entities.end(), [&]( const shared_ptr<Entity>& conel )
		{
			for( const auto& el : m_toRemove )
				if( conel == el )
				{
					m_toRemove.erase( remove( m_toRemove.begin(), m_toRemove.end(), el ), m_toRemove.end() );
					return true;
				}
			return false;
		} ), m_entities.end() );
		m_toRemove.clear();
	}

	void EntityManager::clear()
	{
		for( const auto& entity : m_entities )
			s_entity_removed( entity );
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
