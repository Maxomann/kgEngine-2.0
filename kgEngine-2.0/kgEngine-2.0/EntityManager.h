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
		typedef std::vector<std::shared_ptr<Entity>> EntityContainer;

		static const unsigned int EXPECTED_MAX_ENTITY_COUNT = 200000u;//m_entities will reserve memory for so many objects

	private:
		EntityContainer m_entities;
		EntityContainer m_toRemove;

		EntityContainer::iterator m_findEntity( const std::shared_ptr<Entity>& entity );
	public:
		EntityManager();

		// overwrites entity if it already exists
		// first: returns false in that case
		// second: EntityId of the added Entity
		void addEntity( std::shared_ptr<Entity>& entity );

		// ensures that the entity with id parameter:id does not exist anymore
		// returns false if entity with id did not exist
		void removeEntity( const std::shared_ptr<Entity>& entity );

		// returns nullptr if Entity with id does not exist
		bool doesEntityExist( const std::shared_ptr<Entity>& entity );

		const EntityContainer& getAllEntities()const;

		void updateEntities( Engine& engine, World& world, const sf::Time& frameTime );

		void removeEntitiesOnRemoveList();

		//removes every entity
		void clear();

		unsigned int getEntityCount()const;

	signals:
		Signal<const std::shared_ptr<Entity>&> s_entity_added;
		Signal<const std::shared_ptr<Entity>&> s_entity_removed;
	};
}
