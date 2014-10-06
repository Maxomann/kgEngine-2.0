#pragma once
#include "stdafx.h"
#include "ComponentManager.h"
#include "Blueprint.h"
#include "Engine.h"

namespace kg
{
	class Entity : public ComponentManager
	{
	public:
		typedef unsigned long long Id;

	private:

		const Entity::Id m_id;

	public:
		Entity( const Id& id );

		void initFromBlueprint( Engine& engine, const blueprint::Entity& entity );

		const Id& getId()const;
	};
}
