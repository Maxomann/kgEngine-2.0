#pragma once
#include "stdafx.h"
#include "System.h"
#include "ConfigFile.h"
#include "ResourceManager.h"
#include "Engine.h"

namespace kg
{
	struct Engine;
	class SaveManager;

	class DLL_EXPORT SystemManager
	{
		std::map<size_t, System*> m_systemsByType;
		std::map<double, std::shared_ptr<System>> m_systemsByUpdateImportance;
		/*std::map<Plugin::Id, std::shared_ptr<System>> m_systemsByPluginId;*/
	public:

		// If this function returns true a system of type T has already been registered.
		// This function overwrites the old system with this system parameter.
		bool addSystem( std::shared_ptr<System>& system, size_t realTypeHashCode )
		{
			double updateImportance = system->getUpdateImportance();

			auto it = m_systemsByType.find( realTypeHashCode );

			m_systemsByType[realTypeHashCode] = system.get();
			m_systemsByUpdateImportance[updateImportance] = system;
			/*m_systemsByPluginId[system->getPluginId()] = system;*/

			//it != m_systemsByType.end(); means that a system has been overwritten
			return it != m_systemsByType.end();
		}

		template<class T>
		bool addSystem( std::shared_ptr<System>& system )
		{
			addSystem( system, T::type_hash );
		};

		void initSystemsByImportance( Engine& engine, World& world, SaveManager& saveManager );

		template<class T>
		T* getSystem()
		{
			return static_cast< T* >(m_systemsByType.at( T::type_hash ));
		};

		/*std::shared_ptr<System> getSystemById( const Plugin::Id& id )const;*/
		/*const std::map<Plugin::Id, std::shared_ptr<System>>& getAllSystemsByPluginId()const;*/

		/*template<class T, class CastTo>
		std::shared_ptr<CastTo>& getSystemWithCast()
		{
		return std::static_pointer_cast< CastTo >(getSystem<T>());
		};*/

		void forwardSfmlEventByImportance( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent );
		void updateAllSystemsByImportance( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime );
	};
}
