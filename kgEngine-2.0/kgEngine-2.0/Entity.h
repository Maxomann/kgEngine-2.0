#pragma once
#include "stdafx.h"
#include "ComponentManager.h"
#include "Blueprint.h"
#include "Engine.h"
#include "ComponentMissingException.h"

namespace kg
{
	class DLL_EXPORT Entity : public ComponentManager
	{
	public:
		typedef unsigned long long Id;

	private:

		const Entity::Id m_id;
		blueprint::ComponentValuesByNameByComponentMap m_additionalComponentValues;

	public:
		Entity( const Id& id );

		void initFromBlueprint( Engine& engine,
								const blueprint::Entity& entity,
								const blueprint::ComponentValuesByNameByComponentMap& additionalBlueprintValues
								= blueprint::ComponentValuesByNameByComponentMap() );//componentValuesByNameByComponent

		const blueprint::ComponentValuesByNameByComponentMap& getAdditionalComponentValues()const;

		const Id& getId()const;
	};
}
