#pragma once
#include "stdafx.h"
#include "Position.h"
#include "Graphics.h"

namespace kg
{
	struct ToDrawContainerWorldLayerElement
	{
		bool isSorted = false;
		EntityManager::EntityPointerContainer entities;
	};

	class ToDrawContainer
	{
		std::map<PositionType, ToDrawContainerWorldLayerElement> m_entitiesByWorldLayer;

		void sortContainer( ToDrawContainerWorldLayerElement& toSort );

	public:

		void addEntities( const EntityManager::EntityPointerContainer& entities );
		void removeEntities( const EntityManager::EntityPointerContainer& entities );

		void addEntities_try( const EntityManager::EntityPointerContainer& entities );
		void removeEntities_try( const EntityManager::EntityPointerContainer& entities );

		EntityManager::EntityPointerContainer& getEntitiesFromWorldLayer( const PositionType worldLayer );
	};
}