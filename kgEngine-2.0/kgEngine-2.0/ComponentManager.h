#pragma once
#include "stdafx.h"
#include "Component.h"

namespace kg
{
	class World;

	class DLL_EXPORT ComponentManager// : public boost::noncopyable
	{
	private:
		std::vector<std::unique_ptr<Component>> m_components;
		std::vector<std::pair<size_t, Component*>> m_componentsByType;
		std::vector<Component*> m_componentsByUpdateImportance;

		bool m_componentsSorted = false;
		void m_checkComponentsSortedByUpdateImportance();

		std::vector<std::pair<size_t, Component*>>::const_iterator m_findComponentByType( const std::size_t& hash_code )const;

	public:
		ComponentManager() = default;
		~ComponentManager() = default;
		ComponentManager( const ComponentManager& componentManager ) = delete;
		ComponentManager( ComponentManager&& componentManager ) = default;
		ComponentManager& operator=( const ComponentManager& componentManager ) = delete;
		ComponentManager& operator=( ComponentManager&& componentManager ) = default;

		// If this function returns true a system of type T has already been registered.
		// This function overwrites the old system with the parameter of this function
		// component->init() will be called

		//passed reference will be nullptr after function call!
		void addComponent( std::unique_ptr<Component>& component );

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
			return hasComponent( std::vector<size_t>{ ComponentType::type_hash } );
		};

		//returns true if all components of type ComponentType are registered
		bool hasComponent( const std::vector<size_t>& componentTypes )const;

		bool hasComponent( const Plugin::Id& componentId )const;

		const std::vector<Component*>& getAllComponentsByUpdateImportance()const;
	};
}
