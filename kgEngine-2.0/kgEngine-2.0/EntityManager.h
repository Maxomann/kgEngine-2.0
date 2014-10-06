#pragma once
#include "stdafx.h"
#include "Entity.h"

namespace kg
{
	class EntityManager
	{
		std::unordered_map<Entity::Id, std::shared_ptr<Entity>> m_entities;

		static Entity::Id highestUniqueId;


	public:
		//not unique between servers/clients
		static Entity::Id getUniqueEntityId();

		// overwrites entity if it already exists
		// returns false in that case
		bool addEntity( std::shared_ptr<Entity>& entity );

		// ensures that the entity with id parameter:id does not exist anymore
		// returns false if entity with id did not exist
		bool removeEntity( const Entity::Id& id );

		// returns nullptr if Entity with id does not exist
		std::shared_ptr<Entity> getEntity( const Entity::Id& id );

		//returns all entities that have all components, given in the template parameter, registered
		template<class variadic ComponentType>
		std::vector<std::shared_ptr<Entity>> getEntitiesThatHaveComponent()
		{
			std::vector< std::shared_ptr<Entity>> returnValue;

			for( auto& entity : m_entities )
				if( entity.second->hasComponent<ComponentType>() )
					returnValue.push_back( entity.second );

			return returnValue;
		};

		void updateEntities( Engine& engine, World& world );


		//helper function for creating a new Entity with a unique id
		static std::shared_ptr<Entity> createEntity( Engine& engine, const blueprint::Entity& entity );
	};
}
