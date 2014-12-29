#pragma once
#include "stdafx.h"
#include "Entity.h"
#include "Engine.h"
#include "SaveComponent.h"

namespace kg
{
	class DLL_EXPORT EntityFactory
	{
		Entity::Id m_highestUniqueId = 0;

	public:
		//not unique between servers/clients
		Entity::Id getUniqueEntityId();
		void setLowestUniqueEntityId( const Entity::Id& id );

		// helper function for creating a new Entity with a unique id
		// A Save component will be added to the entity
		std::shared_ptr<Entity> createNewEntity( Engine& engine,
												 const int& entityBlueprintId );
		// helper function for creating a specific unique id
		// A Save component will be added to the entity
		std::shared_ptr<Entity> createNewEntity( Engine& engine,
												 const int& entityBlueprintId,
												 const Entity::Id& uniqueId );
	};
}
