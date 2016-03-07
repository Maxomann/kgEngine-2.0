#include "EntityManager.h"

using namespace std;
using namespace sf;

namespace kg
{
	Entity* EntityManager::addEntity( Entity&& entity )
	{
		m_entities.push_back( move( entity ) );
		Entity* temp = &m_entities.back();
		m_addedEntities.push_back( temp );
		return &m_entities.back();
	}

	void EntityManager::addEntities( std::vector<Entity>&& entities )
	{
		for( auto& el : entities )
			addEntity( move( el ) );
	}

	void EntityManager::removeEntity( Entity* entity )
	{
		auto it = find( m_addedEntities.begin(), m_addedEntities.end(), entity );
		if( it != m_addedEntities.end() )
		{
			m_addedEntities.erase( remove( m_addedEntities.begin(), m_addedEntities.end(), *it ) );
			m_entities.erase( remove( m_entities.begin(), m_entities.end(), **it ) );
		}
		else
			m_removedEntites.push_back( entity );
	}

	void EntityManager::removeEntities( const EntityPointerContainer& entities )
	{
		for( auto& el : entities )
			removeEntity( el );
	}

	bool EntityManager::doesEntityExist( const Entity* entity ) const
	{
		return std::find( cbegin( m_entities ), cend( m_entities ), *entity ) != m_entities.cend();
	}

	void EntityManager::updateEntities( Engine& engine, World& world, const sf::Time& frameTime )
	{
		for( auto& entity : m_entities )
			entity.updateAllComponentsByImportance( engine, world, frameTime );
	}

	void EntityManager::performAddRemoveActions()
	{
		s_entities_removed( m_removedEntites );
		s_entities_added( m_addedEntities );

		m_entities.remove_if( [&]( const Entity& el )
		{
			return any_of( m_removedEntites.begin(), m_removedEntites.end(), [&]( const Entity* entity )
			{
				return entity == &el;
			} );
		} );

		m_addedEntities.clear();
		m_removedEntites.clear();
	}

	void EntityManager::clear()
	{
		for( auto& el : m_entities )
			removeEntity( &el );
		performAddRemoveActions();
	}

	unsigned int EntityManager::getEntityCount() const
	{
		return m_entities.size();
	}

	EntityManager::EntityContainer::iterator EntityManager::m_findEntity( const Entity& entity )
	{
		return std::find( begin( m_entities ), end( m_entities ), entity );
	}
}