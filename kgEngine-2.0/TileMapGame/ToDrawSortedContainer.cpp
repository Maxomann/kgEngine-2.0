#include "ToDrawSortedContainer.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	void ToDrawSortedContainer::sortContainer( ToDrawSortedContainerWorldLayerElement& toSort )
	{
		auto& toSortEntities = toSort.entities;

		std::sort( toSortEntities.begin(), toSortEntities.end(), [](
			const Entity* lhs,
			const Entity* rhs )
		{
			const auto& positionLeft = lhs->getComponent<Transformation>()->getPosition();
			const auto& positionRight = rhs->getComponent<Transformation>()->getPosition();

			if( positionRight.zValue > positionLeft.zValue )
				return true;
			else if( positionRight.zValue == positionLeft.zValue && positionRight.y > positionLeft.y )
				return true;
			else if( positionRight.y == positionLeft.y && positionRight.x > positionLeft.x )
				return true;

			return false;
		} );

		toSort.isSorted = true;
	}

	void ToDrawSortedContainer::addEntities( const EntityManager::EntityPointerContainer& entities )
	{
		for( auto& entity : entities )
		{
			auto worldLayer = entity->getComponent<Transformation>()->getPosition().worldLayer;
			auto& el = m_entitiesByWorldLayer[worldLayer];
			el.entities.push_back( entity );
			el.isSorted = false;
		}
	}

	void ToDrawSortedContainer::removeEntities( const EntityManager::EntityPointerContainer& entities )
	{
		for( auto& entity : entities )
		{
			auto worldLayer = entity->getComponent<Transformation>()->getPosition().worldLayer;
			auto& el = m_entitiesByWorldLayer[worldLayer];

			//remove
			el.entities.erase( std::remove( el.entities.begin(), el.entities.end(), entity ), el.entities.end() );
		}
	}

	void ToDrawSortedContainer::addEntities_try( const EntityManager::EntityPointerContainer& entities )
	{
		EntityManager::EntityPointerContainer validEntities;

		for( auto& entity : entities )
			if( entity->hasComponent<Graphics>() )
				validEntities.push_back( entity );

		addEntities( validEntities );
	}

	void ToDrawSortedContainer::removeEntities_try( const EntityManager::EntityPointerContainer& entities )
	{
		EntityManager::EntityPointerContainer validEntities;

		for( auto& entity : entities )
			if( entity->hasComponent<Graphics>() )
				validEntities.push_back( entity );

		removeEntities( validEntities );
	}

	EntityManager::EntityPointerContainer& ToDrawSortedContainer::getEntitiesFromWorldLayer( const PositionType worldLayer )
	{
		auto& el = m_entitiesByWorldLayer[worldLayer];

		if( !el.isSorted )
			sortContainer( el );

		if( !el.isSorted )
			throw exception();

		return el.entities;
	}
}