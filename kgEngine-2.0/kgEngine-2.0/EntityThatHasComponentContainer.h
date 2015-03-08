#pragma once
#include "stdafx.h"
#include "World.h"
#include "Callback.h"

namespace kg
{
	template< class ComponentType >
	class EntityThatHasComponentContainer : public CallbackReciever
	{
		EntityManager::EntityContainer m_entities;

		void m_onEntityAddedToWorld( const std::shared_ptr<Entity>& entity )
		{
			if( entity->hasComponent<ComponentType>() )
				m_entities.insert( entity );
		}
		void m_onEntityRemovedFromWorld( const std::shared_ptr<Entity>& entity )
		{
			if( entity->hasComponent<ComponentType>() )
				m_entities.erase( entity );
		}

	protected:
		void init( World& world )
		{
			m_connectToSignal( world.s_entity_added, &EntityThatHasComponentContainer::m_onEntityAddedToWorld );
			m_connectToSignal( world.s_entity_removed, &EntityThatHasComponentContainer::m_onEntityRemovedFromWorld );
		}

	public:
		const EntityManager::EntityContainer& getEntitiesThatHaveComponent()const
		{
			return m_entities;
		}
	};
}
