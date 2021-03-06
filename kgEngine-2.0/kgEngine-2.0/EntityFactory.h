#pragma once
#include "stdafx.h"
#include "Engine.h"
#include "SaveComponent.h"

namespace kg
{
	//Workaround
	template<class T>
	void fill_vec( std::vector<std::unique_ptr<Component>>& vec )
	{
		vec.push_back( std::move( std::make_unique<T>() ) );
	}

	template<class T1, class T2, class ... Tn>
	void fill_vec( std::vector<std::unique_ptr<Component>>& vec )
	{
		fill_vec<T1>( vec );
		fill_vec<T2, Tn...>( vec );
	}

	class DLL_EXPORT EntityFactory : public boost::noncopyable
	{
		std::mutex m_highestUniqueIdMutex;
		Entity::Id m_highestUniqueId = 0;// Thread safe

	public:
		// Thread safe
		// not unique between servers/clients
		Entity::Id getUniqueEntityId();
		// Thread safe
		void setLowestUniqueEntityId( const Entity::Id& id );

		// Thread safe
		// helper function for creating a new Entity with a unique id
		// A Save component will be added to the entity
		Entity createNewSaveableEntity( Engine& engine,
										const World& world,
										const int& entityBlueprintId );

		// Thread safe
		// helper function for creating a specific unique id
		// A Save component will be added to the entity
		Entity createNewSaveableEntity( Engine& engine,
										const World& world,
										const int& entityBlueprintId,
										const Entity::Id& uniqueId );

		// temporary Entities are not saveable
		// the preInit function of its components will not be called
		template<class variadic ComponentsType>
		Entity createNewTemporaryEntity( Engine& engine, const World& world )
		{
			Entity entity;
			std::vector<std::unique_ptr<Component>> vec;
			fill_vec<ComponentsType...>( vec );

			for( auto& el : vec )
				entity.addComponent( el );
			entity.initComponentsByImportance( engine, world );

			return entity;
		};
	};
}
