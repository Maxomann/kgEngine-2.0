#pragma once
#include "stdafx.h"
#include "Entity.h"
#include "Callback.h"
#include "EntityFactory.h"

namespace kg
{
	class DLL_EXPORT EntityManager : public EntityFactory
	{
		std::vector<std::shared_ptr<Entity>> m_entities;

		std::map<size_t, std::list<std::shared_ptr<Entity>>> m_entitiesByComponentsTheyHave;

		void addEntityReferencesByComponentType( const std::shared_ptr<Entity>& entity );
		void removeEntityReferencesByComponentType( const std::shared_ptr<Entity>& entity );

		std::vector<std::shared_ptr<Entity>>::iterator m_findEntity( const std::shared_ptr<Entity>& entity );
	public:
		// overwrites entity if it already exists
		// first: returns false in that case
		// second: EntityId of the added Entity
		bool addEntity( std::shared_ptr<Entity>& entity );

		// ensures that the entity with id parameter:id does not exist anymore
		// returns false if entity with id did not exist
		bool removeEntity( const std::shared_ptr<Entity>& entity );


		// returns nullptr if Entity with id does not exist
		bool doesEntityExist( const std::shared_ptr<Entity>& entity );

		//returns all entities that have all components, given in the template parameter, registered
		template<class /*variadic*/ ComponentType>
		const std::list<std::shared_ptr<Entity>>& getEntitiesThatHaveComponent()
		{
			return m_entitiesByComponentsTheyHave[typeid(ComponentType).hash_code()];
		};

		const std::vector<std::shared_ptr<Entity>>& getAllEntities()const;

		void updateEntities( Engine& engine, World& world, const sf::Time& frameTime );

		//removes every entity
		void clear();

		unsigned int getEntityCount()const;

	signals:
		Signal<std::shared_ptr<Entity>&> s_entity_added;
		Signal<std::shared_ptr<Entity>&> s_entity_removed;
	};
}
