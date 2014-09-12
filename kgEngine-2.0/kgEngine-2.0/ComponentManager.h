#pragma once
#include "stdafx.h"
#include "Component.h"

namespace kg
{
	class ComponentManager
	{
		std::map<size_t, std::shared_ptr<Component>> m_componentsByType;
		std::map<double, std::shared_ptr<Component>> m_componentsByUpdateImportance;
	public:
		// If this function returns true a system of type T has already been registered.
		// This function overwrites the old system with the parameter of this function
		template<class T>
		bool addComponent( std::shared_ptr<Component>& component );

		template<class T>
		std::shared_ptr<Component>& getComponent();

		template<class T, class CastTo>
		std::shared_ptr<CastTo>& getComponentWithCast();

		void updateAllComponentsByImportance( Engine& engine, World& world );
	};
}