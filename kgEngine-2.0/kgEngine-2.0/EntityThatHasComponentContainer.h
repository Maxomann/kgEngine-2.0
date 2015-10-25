#pragma once
#include "stdafx.h"
#include "World.h"
#include "Callback.h"

namespace kg
{
	template< class ComponentType >
	class EntityThatHasComponentContainer : public CallbackReciever
	{
		EntityManager::EntityPointerContainer m_entities;

		void m_onEntityAddedToWorld( const Entity* entity )
		{
			if( entity->hasComponent<ComponentType>() )
				m_entities.push_back( entity );
		}
		void m_onEntityRemovedFromWorld( const Entity* entity )
		{
			if( entity->hasComponent<ComponentType>() )
				m_entities.erase( find( m_entities.begin(), m_entities.end(), entity ));
		}

	protected:
		void init( World& world )
		{
			m_connectToSignal( world.s_entity_added, &EntityThatHasComponentContainer::m_onEntityAddedToWorld );
			m_connectToSignal( world.s_entity_removed, &EntityThatHasComponentContainer::m_onEntityRemovedFromWorld );
		}

	public:
		const EntityManager::EntityPointerContainer& getEntitiesThatHaveComponent()const
		{
			return m_entities;
		}
	};
}
