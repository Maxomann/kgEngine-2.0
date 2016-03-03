#include "ToDrawContainer.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	void ToDrawContainer::sortContainer( ToDrawContainerWorldLayerElement& toSort )
	{
		auto& toSortEntities = toSort.entities;

		std::sort( toSortEntities.begin(), toSortEntities.end(), [](
			const Entity* lhs,
			const Entity* rhs )
		{
			const auto& zValueLeft = lhs->getComponent<Graphics>()->getZValue();
			const auto& zValueRight = rhs->getComponent<Graphics>()->getZValue();

			if( zValueRight > zValueLeft )
				return true;

			return false;
		} );

		toSort.isSorted = true;
	}

	void ToDrawContainer::addEntities( const EntityManager::EntityPointerContainer& entities )
	{
		for( auto& entity : entities )
		{
			auto worldLayer = entity->getComponent<Transformation>()->getPosition().worldLayer;
			auto& el = m_entitiesByWorldLayer[worldLayer];
			el.entities.push_back( entity );
			el.isSorted = false;
		}
	}

	void ToDrawContainer::removeEntities( const EntityManager::EntityPointerContainer& entities )
	{
		for( auto& entity : entities )
		{
			auto worldLayer = entity->getComponent<Transformation>()->getPosition().worldLayer;
			auto& el = m_entitiesByWorldLayer[worldLayer];

			//remove
			el.entities.erase( std::remove( el.entities.begin(), el.entities.end(), entity ), el.entities.end() );
		}
	}

	void ToDrawContainer::addEntities_try( const EntityManager::EntityPointerContainer& entities )
	{
		EntityManager::EntityPointerContainer validEntities;

		for( auto& entity : entities )
			if( entity->hasComponent<Graphics>() )
				validEntities.push_back( entity );

		addEntities( validEntities );
	}

	void ToDrawContainer::removeEntities_try( const EntityManager::EntityPointerContainer& entities )
	{
		EntityManager::EntityPointerContainer validEntities;

		for( auto& entity : entities )
			if( entity->hasComponent<Graphics>() )
				validEntities.push_back( entity );

		removeEntities( validEntities );
	}

	EntityManager::EntityPointerContainer& ToDrawContainer::getEntitiesFromWorldLayer( const PositionType worldLayer )
	{
		auto& el = m_entitiesByWorldLayer[worldLayer];

		if( !el.isSorted )
			sortContainer( el );

		if( !el.isSorted )
			throw exception();

		return el.entities;
	}
}