#pragma once
#include "stdafx.h"
#include "ComponentManager.h"
#include "ComponentMissingException.h"

namespace kg
{
	class EntitySaveInformation;

	class DLL_EXPORT Entity : public ComponentManager
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
