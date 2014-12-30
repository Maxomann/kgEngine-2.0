#pragma once
#include "stdafx.h"
#include "Entity.h"
#include "Callback.h"
#include "EntityFactory.h"

namespace kg
{
	class DLL_EXPORT EntityManager : public EntityFactory
	{
		std::unordered_map<Entity*, std::shared_ptr<Entity>> m_entities;

	public:
		// overwrites entity if it already exists
		// first: returns false in that case
		// second: EntityId of the added Entity
		std::pair<bool, std::shared_ptr<Entity>> addEntity( std::shared_ptr<Entity>& entity );

		// ensures that the entity with id parameter:id does not exist anymore
		// returns false if entity with id did not exist
		bool removeEntity( const std::shared_ptr<Entity>& entity );

		// returns nullptr if Entity with id does not exist
		std::shared_ptr<Entity> getEntity( const std::shared_ptr<Entity>& entity );

		//returns all entities that have all components, given in the template parameter, registered
		template<class /*variadic*/ ComponentType>
		std::vector<std::shared_ptr<Entity>> getEntitiesThatHaveComponent()
		{
			std::vector<std::shared_ptr<Entity>> returnValue;

			for( auto& entity : m_entities )
				if( entity.second->hasComponent<ComponentType>() )
					returnValue.push_back( entity.second );

			return returnValue;
		};

		std::vector<std::shared_ptr<Entity>> getAllEntities();

		void updateEntities( Engine& engine, World& world, const sf::Time& frameTime );

		//removes every entity
		void clear();

	signals:
		Signal<std::shared_ptr<Entity>&> s_entity_added;
		Signal<std::shared_ptr<Entity>&> s_entity_removed;
	};
}
