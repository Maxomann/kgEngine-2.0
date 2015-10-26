#include "EntityManager.h"

using namespace std;
using namespace sf;

namespace kg
{

	Entity* EntityManager::addEntity( Entity&& entity )
	{
		m_entities.push_back( move(entity) );
		Entity* temp = &m_entities.back();
		s_entity_added( temp );
		return &m_entities.back();
	}

	void EntityManager::removeEntity( Entity* entity )
	{
		s_entity_removed( entity );
		m_toRemove.push_back( entity );
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

	void EntityManager::removeEntitiesOnRemoveList()
	{
		s_removeEntitiesOnRemoveList();

		auto condition = [&]( const Entity& conel )
		{
			for( const auto& el : m_toRemove )
				if( conel == *el )
				{
					m_toRemove.erase( remove( m_toRemove.begin(), m_toRemove.end(), el ), m_toRemove.end() );
					return true;
				}
			return false;
		};

		m_entities.remove_if( condition );
		if( m_toRemove.size() != 0 )
			throw exception();
		m_toRemove.clear();
	}

	void EntityManager::clear()
	{
		removeEntitiesOnRemoveList();
		for( auto& entity : m_entities )
			removeEntity( &entity );
		removeEntitiesOnRemoveList();
	}

	const EntityManager::EntityContainer& EntityManager::getAllEntities() const
	{
		return m_entities;
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
