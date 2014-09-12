#pragma once
#include "stdafx.h"
#include "Entity.h"

namespace kg
{
	class EntityContainer
	{
		std::unordered_map<Entity::Id, std::shared_ptr<Entity>> m_entities;

		static Entity::Id highestUniqueId;

	public:
		//not guaranteed to be unique between servers/clients
		static Entity::Id getUniqueEntityId();

		// overwrites entity if it already exists
		// returns false in that case
		bool addEntity( std::shared_ptr<Entity>& entity );

		// ensures that the entity with id parameter:id does not exist anymore
		// returns false if entity with id did not exist
		bool removeEntity( const Entity::Id& id );

		// returns nullptr if Entity with id does not exist
		std::shared_ptr<Entity> getEntity( const Entity::Id& id );

		void updateEntities( Engine& engine, World& world );
	};
}
