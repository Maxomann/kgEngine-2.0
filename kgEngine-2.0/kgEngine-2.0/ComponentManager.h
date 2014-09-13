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
		// component->init() will be called
		template<class T>
		bool addComponent( std::shared_ptr<Component>& component )
		{
			size_t typeId = typeid(T).hash_code();
			double updateImportance = component->getUpdateImportance();

			auto it = m_componentsByType.find( typeId );

			m_componentsByType[typeId] = component;
			m_componentsByUpdateImportance[updateImportance] = component;

			//it != m_systemsByType.end(); means that a system has been overwritten
			return it != m_componentsByType.end();
		};

		void initComponents( Engine& engine );

		template<class T>
		std::shared_ptr<Component>& getComponent()
		{
			auto it = m_componentsByType.find( typeid(T).hash_code() );
			if( it == m_componentsByType.end() )
				return nullptr;
			else
				return it->second;
		};

		template<class T, class CastTo>
		std::shared_ptr<CastTo>& getComponentWithCast()
		{
			return std::static_pointer_cast< CastTo >(getComponent<T>());
		};

		void updateAllComponentsByImportance( Engine& engine, World& world );

		//returns true if all components of type ComponentType are registered
		template<class variadic ComponentType>
		bool hasComponent()const
		{
			std::vector<size_t> componentTypes{ typeid(ComponentType).hash_code()... };

			for( const auto& type : componentTypes )
				if( m_componentsByType.find( type ) == m_componentsByType.end() )
					return false;
			return true;
		};
	};
}
