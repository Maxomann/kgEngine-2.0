#include "EntityManager.h"

using namespace std;
using namespace sf;

namespace kg
{
	bool EntityManager::addEntity( std::shared_ptr<Entity>& entity )
	{
		bool didExist = doesEntityExist( entity );

		m_entities.push_back( entity );

		addEntityReferencesByComponentType( entity );
		s_entity_added( entity );

		//return false if Entity already existed
		return !didExist;
	}

	bool EntityManager::removeEntity( const std::shared_ptr<Entity>& entity )
	{
		auto it = m_findEntity( entity );

		bool didExist = false;

		if( it != m_entities.end() )
		{
			s_entity_removed( *it );
			didExist = true;
			removeEntityReferencesByComponentType( entity );
			m_entities.erase( m_findEntity( entity ) );
		}

		return didExist;
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
			auto& vec = m_entitiesByComponentsTheyHave[comp.first];
			auto it = find( begin( vec ), end( vec ), entity );
			if( it != vec.end() )
				vec.erase( it );
		}
	}

	std::vector<std::shared_ptr<Entity>>::iterator EntityManager::m_findEntity( const std::shared_ptr<Entity>& entity )
	{
		return find( begin( m_entities ), end( m_entities ), entity );
	}

}
