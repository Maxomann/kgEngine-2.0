#pragma once
#include "stdafx.h"
#include "ComponentManager.h"
#include "ComponentMissingException.h"

namespace kg
{
	class DLL_EXPORT Entity : public ComponentManager
	{
	public:
		typedef unsigned long long Id;

		Entity() = default;
		~Entity() = default;
		Entity( const Entity& entity ) = delete;
		Entity( Entity&& entity ) = default;

		Entity& operator=( const Entity& entity ) = delete;
		Entity& operator=( Entity&& entity ) = default;

		bool operator==( const Entity& entity )const;// returns true if (&entity == this)
	};
}
