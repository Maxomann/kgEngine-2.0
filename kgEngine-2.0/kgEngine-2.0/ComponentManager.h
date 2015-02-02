#pragma once
#include "stdafx.h"
#include "Component.h"

namespace kg
{
	class World;

	namespace workaround
	{
		template<typename T>
		void fill( std::vector<size_t>& vec )
		{
			vec.push_back( typeid(T).hash_code() );
		}

		template <typename T1, typename T2, typename variadic Tn>
		void fill( std::vector<size_t>& vec )
		{
			fill<T1>( vec );
			fill<T2, Tn>( vec );
		}
	}

	class DLL_EXPORT ComponentManager
	{
		std::map<size_t, std::shared_ptr<Component>> m_componentsByType;
		std::map<double, std::shared_ptr<Component>> m_componentsByUpdateImportance;
		std::map<Plugin::Id, std::shared_ptr<Component>> m_componentsByPluginId;
	public:
		// If this function returns true a system of type T has already been registered.
		// This function overwrites the old system with the parameter of this function
		// component->init() will be called

		bool addComponent( std::shared_ptr<Component>& component, size_t realTypeHashCode )
		{
			double updateImportance = component->getUpdateImportance();

			auto it = m_componentsByType.find( realTypeHashCode );

			m_componentsByType[realTypeHashCode] = component;
			m_componentsByUpdateImportance[updateImportance] = component;
			m_componentsByPluginId[component->getPluginId()] = component;

			//if [it != m_systemsByType.end();] a system has been overwritten
			return it != m_componentsByType.end();
		}

		template<class T>
		bool addComponent( std::shared_ptr<Component>& component )
		{
			size_t typeId = typeid(T).hash_code();
			return addComponent( component, typeId );
		};

		void initComponentsByImportance( Engine& engine, World& world );

		template<class T>
		std::shared_ptr<T> getComponent()
		{
			auto it = m_componentsByType.find( typeid(T).hash_code() );
			if( it == m_componentsByType.end() )
				return std::static_pointer_cast< T >(std::shared_ptr<void>());
			else
				return std::static_pointer_cast< T >(it->second);
		};

		void updateAllComponentsByImportance( Engine& engine, World& world, const sf::Time& frameTime );

		//returns true if all components of type ComponentType are registered
		template<class /*variadic*/ ComponentType>
		bool hasComponent()const
		{
			//INTERNAL COMPILER ERROR : COMPILER BUG
			// 			std::vector<size_t> componentTypes { (typeid(ComponentType).hash_code())... };

			//WORKAROUND
			std::vector<size_t> componentTypes;// { typeid(ComponentType).hash_code() };
			workaround::fill<ComponentType>( componentTypes );

			return hasComponent( componentTypes );
		};

		//returns true if all components of type ComponentType are registered
		bool hasComponent( const std::vector<size_t>& componentTypes )const;

		const std::map<size_t, std::shared_ptr<Component>>& getAllComponentsByTypeHash()const;

		const std::map<Plugin::Id, std::shared_ptr<Component>>& getAllComponentsByPluginId()const;
	};
}
