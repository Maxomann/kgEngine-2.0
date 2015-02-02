#pragma once
#include "stdafx.h"
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
		std::shared_ptr<Entity> createNewSaveableEntity( Engine& engine,
														 World& world,
														 const int& entityBlueprintId );
		// helper function for creating a specific unique id
		// A Save component will be added to the entity
		std::shared_ptr<Entity> createNewSaveableEntity( Engine& engine,
														 World& world,
														 const int& entityBlueprintId,
														 const Entity::Id& uniqueId );

		// temporary Entities are not saveable
		// the preInit function of its components will not be called
		template<class variadic ComponentsType>
		std::shared_ptr<Entity> createNewTemporaryEntity( Engine& engine, World& world )
		{
			auto entity = std::make_shared<Entity>();
			std::vector<std::pair<size_t, std::shared_ptr<Component>>> vec{ std::make_pair(typeid(ComponentsType).hash_code(), std::static_pointer_cast< Component >(std::make_shared<ComponentsType>()))... };
			for( auto& el : vec )
				entity->addComponent( el.second, el.first );
			entity->initComponentsByImportance( engine, world );

			return entity;
		};
	};
}
