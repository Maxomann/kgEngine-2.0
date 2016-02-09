#pragma once
#include "stdafx.h"
#include "Position.h"
#include "Graphics.h"

namespace kg
{
	struct ToDrawSortedContainerWorldLayerElement
	{
		bool isSorted = false;
		EntityManager::EntityPointerContainer entities;
	};

	class ToDrawSortedContainer
	{
		std::map<PositionType, ToDrawSortedContainerWorldLayerElement> m_entitiesByWorldLayer;

		void sortContainer( ToDrawSortedContainerWorldLayerElement& toSort );

	public:

		void addEntities( const EntityManager::EntityPointerContainer& entities );
		void removeEntities( const EntityManager::EntityPointerContainer& entities );

		void addEntities_try( const EntityManager::EntityPointerContainer& entities );
		void removeEntities_try( const EntityManager::EntityPointerContainer& entities );

		EntityManager::EntityPointerContainer& getEntitiesFromWorldLayer( const PositionType worldLayer );
	};
}