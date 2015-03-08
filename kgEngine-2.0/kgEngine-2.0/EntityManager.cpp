#include "EntityManager.h"

using namespace std;
using namespace sf;

namespace kg
{
	void EntityManager::addEntity( std::shared_ptr<Entity>& entity )
	{
		m_entities.insert( entity );

		/*addEntityReferencesByComponentType( entity );*/
		s_entity_added( entity );
	}

	void EntityManager::removeEntity( const std::shared_ptr<Entity>& entity )
	{
		EntityContainer::iterator it = m_findEntity( entity );

		if( it != m_entities.end() )
		{
			s_entity_removed( *it );
			/*removeEntityReferencesByComponentType( entity );*/
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
		/*m_entitiesByComponentsTheyHave.clear();*/
	}

	const EntityManager::EntityContainer& EntityManager::getAllEntities() const
	{
		return m_entities;
	}

	unsigned int EntityManager::getEntityCount() const
	{
		return m_entities.size();
	}

	//void EntityManager::addEntityReferencesByComponentType( const std::shared_ptr<Entity>& entity )
	//{
	//	for( const auto& comp : entity->getAllComponentsByTypeHash() )
	//		m_entitiesByComponentsTheyHave[comp.first].insert( entity );
	//}

	//void EntityManager::removeEntityReferencesByComponentType( const std::shared_ptr<Entity>& entity )
	//{
	//	/*SLOW! SLOW! SLOW AS HELL!!!*/
	//	for( const auto& comp : entity->getAllComponentsByTypeHash() )
	//	{
	//		auto& container = m_entitiesByComponentsTheyHave[comp.first];
	//		/*auto it = find( begin( container ), end( container ), entity.get() );*/
	//		auto it = container.find( entity );
	//		if( it != container.end() )
	//			container.erase( it );
	//	}
	//}

	EntityManager::EntityContainer::iterator EntityManager::m_findEntity( const std::shared_ptr<Entity>& entity )
	{
		return m_entities.find( entity );
	}
}