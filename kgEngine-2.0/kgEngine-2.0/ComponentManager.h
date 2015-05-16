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
			vec.push_back( T::type_hash );
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
	private:
		std::vector<std::shared_ptr<Component>> m_components;
		std::vector<std::pair<size_t, Component*>> m_componentsByType;
		std::vector<Component*> m_componentsByUpdateImportance;

		bool m_componentsSorted = false;
		inline void m_checkComponentsSortedByUpdateImportance();

		inline std::vector<std::pair<size_t, Component*>>::const_iterator m_findComponentByType( const std::size_t& hash_code )const;

	public:
		// If this function returns true a system of type T has already been registered.
		// This function overwrites the old system with the parameter of this function
		// component->init() will be called

		void addComponent( std::shared_ptr<Component>& component, const std::size_t& realTypeHashCode )
		{
			auto it = m_findComponentByType( realTypeHashCode );

			m_components.emplace_back( component );
			m_componentsByType.emplace_back( realTypeHashCode, component.get() );
			m_componentsByUpdateImportance.emplace_back( component.get() );
		}

		template<class T>
		void addComponent( std::shared_ptr<Component>& component )
		{
			addComponent( component, T::type_hash );
		};

		void initComponentsByImportance( Engine& engine, World& world );

		template<class T>
		inline T* getComponent()const
		{
			return static_cast< T* >(m_findComponentByType( T::type_hash )->second);
		};

		template<class T>
		inline T* getComponentTry()const
		{
			auto it = m_findComponentByType( T::type_hash );
			if( it == m_componentsByType.end() )
				return static_cast< T* >(nullptr);
			else
				return static_cast< T* >(it->second);
		};

		void updateAllComponentsByImportance( Engine& engine, World& world, const sf::Time& frameTime );

		//returns true if all components of type ComponentType are registered
		template<class /*variadic*/ ComponentType>
		bool hasComponent()const
		{
			//INTERNAL COMPILER ERROR : COMPILER BUG
			// 			std::vector<size_t> componentTypes { (typeid(ComponentType).hash_code())... };

			//WORKAROUND
			/*std::vector<size_t> componentTypes;// { typeid(ComponentType).hash_code() };
			workaround::fill<ComponentType>( componentTypes );
			//componentTypes.push_back( typeid(ComponentType).hash_code() );*/

			return hasComponent( { ComponentType::type_hash } );
		};

		//returns true if all components of type ComponentType are registered
		bool hasComponent( const std::vector<size_t>& componentTypes )const;

		const std::vector<std::pair<size_t, Component*>>& getAllComponentsByTypeHash()const;

		/*const std::unordered_map<Plugin::Id, std::shared_ptr<Component>>& getAllComponentsByPluginId()const;*/
	};
}
