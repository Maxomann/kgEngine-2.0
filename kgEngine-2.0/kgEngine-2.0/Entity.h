#pragma once
#include "stdafx.h"
#include "ComponentManager.h"

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

		const Id& getId()const;
		
	};
}
