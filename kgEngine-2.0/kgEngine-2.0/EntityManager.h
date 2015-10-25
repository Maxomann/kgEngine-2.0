#pragma once
#include "stdafx.h"
#include "Entity.h"
#include "Callback.h"
#include "EntityFactory.h"

namespace kg
{
	class DLL_EXPORT EntityManager : public EntityFactory
	{
	public:
		typedef std::list<Entity> EntityContainer;//needs to be list, because references(pointers) must not be invalidated
		typedef std::vector<Entity*> EntityPointerContainer;

	private:
		EntityContainer m_entities;
		EntityPointerContainer m_toRemove;

		EntityContainer::iterator m_findEntity( const Entity& entity );
	public:

		// overwrites entity if it already exists
		// first: returns false in that case
		// second: EntityId of the added Entity
		// return value: a reference to the entity in EntityManager
		Entity* addEntity( Entity&& entity );

		// ensures that the entity with id parameter:id does not exist anymore
		// returns false if entity with id did not exist
		void removeEntity( Entity* entity );

		// returns nullptr if Entity with id does not exist
		bool doesEntityExist( const Entity* entity )const;

		const EntityContainer& getAllEntities()const;

		void updateEntities( Engine& engine, World& world, const sf::Time& frameTime );

		void removeEntitiesOnRemoveList();

		//removes every entity
		void clear();

		unsigned int getEntityCount()const;

	signals:
		Signal<Entity*> s_entity_added;
		Signal<Entity*> s_entity_removed;
	};
}
