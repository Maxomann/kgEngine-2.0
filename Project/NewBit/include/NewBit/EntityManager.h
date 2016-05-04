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
		EntityPointerContainer m_addedEntities;
		EntityPointerContainer m_removedEntites;

		EntityContainer::iterator m_findEntity( const Entity& entity );
	public:

		// overwrites entity if it already exists
		// return value: a pointer to the added entity
		Entity* addEntity( Entity&& entity );
		void addEntities( std::vector<Entity>&& entities );

		void removeEntity( Entity* entity );
		void removeEntities( const EntityPointerContainer& entities );

		bool doesEntityExist( const Entity* entity )const;

		void updateEntities( Engine& engine, World& world, const sf::Time& frameTime );
		void performAddRemoveActions();

		//removes every entity
		void clear();

		unsigned int getEntityCount()const;

	signals:
		Signal<const EntityPointerContainer&> s_entities_added;
		Signal<const EntityPointerContainer&> s_entities_removed;
	};

	using EntityPointerContainer = EntityManager::EntityPointerContainer;
}
