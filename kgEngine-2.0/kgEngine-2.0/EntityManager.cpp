#include "EntityManager.h"

using namespace std;
using namespace sf;

namespace kg
{
	void EntityManager::addEntity( std::shared_ptr<Entity>& entity )
	{
		m_entities.push_back( entity );

		addEntityReferencesByComponentType( entity );
		s_entity_added( entity );
	}

	void EntityManager::removeEntity( const std::shared_ptr<Entity>& entity )
	{
		auto it = m_findEntity( entity );

		if( it != m_entities.end() )
		{
			s_entity_removed( *it );
			removeEntityReferencesByComponentType( entity );
			m_entities.erase( it );
		}
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
		m_entitiesByComponentsTheyHave.clear();
	}

	const std::vector<std::shared_ptr<Entity>>& EntityManager::getAllEntities()const
	{
		return m_entities;
	}

	unsigned int EntityManager::getEntityCount() const
	{
		return m_entities.size();
	}

	void EntityManager::addEntityReferencesByComponentType( const std::shared_ptr<Entity>& entity )
	{
		for( const auto& comp : entity->getAllComponentsByTypeHash() )
			m_entitiesByComponentsTheyHave[comp.first].push_back( entity );
	}

	void EntityManager::removeEntityReferencesByComponentType( const std::shared_ptr<Entity>& entity )
	{
		for( const auto& comp : entity->getAllComponentsByTypeHash() )
		{
			auto& container = m_entitiesByComponentsTheyHave[comp.first];
			auto it = find( begin( container ), end( container ), entity );
			if( it != container.end() )
				container.erase( it );
		}
	}

	std::vector<std::shared_ptr<Entity>>::iterator EntityManager::m_findEntity( const std::shared_ptr<Entity>& entity )
	{
		return find( begin( m_entities ), end( m_entities ), entity );
	}

}
