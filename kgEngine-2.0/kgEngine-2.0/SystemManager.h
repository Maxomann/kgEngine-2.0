#pragma once
#include "stdafx.h"
#include "System.h"

namespace kg
{
	class DLL_EXPORT SystemManager
	{
		std::map<size_t, std::shared_ptr<System>> m_systemsByType;
		std::map<double, std::shared_ptr<System>> m_systemsByUpdateImportance;
	public:
		// If this function returns true a system of type T has already been registered.
		// This function overwrites the old system with the parameter of this function.

		bool addSystem( std::shared_ptr<System>& system, size_t realTypeHashCode )
		{
			double updateImportance = system->getUpdateImportance();

			auto it = m_systemsByType.find( realTypeHashCode );

			m_systemsByType[realTypeHashCode] = system;
			m_systemsByUpdateImportance[updateImportance] = system;

			//it != m_systemsByType.end(); means that a system has been overwritten
			return it != m_systemsByType.end();
		}

		template<class T>
		bool addSystem( std::shared_ptr<System>& system )
		{
			size_t typeId = typeid(T).hash_code();
			addSystem( system, typeId );
		};

		void initSystemsByImportance( Engine& engine, World& world );

		template<class T>
		std::shared_ptr<T> getSystem()
		{
			auto it = m_systemsByType.find( typeid(T).hash_code() );
			if( it == m_systemsByType.end() )
				return std::static_pointer_cast< T >(std::shared_ptr<void>());
			else
				return static_pointer_cast<T>(it->second);
		};

		/*template<class T, class CastTo>
		std::shared_ptr<CastTo>& getSystemWithCast()
		{
			return std::static_pointer_cast< CastTo >(getSystem<T>());
		};*/

		void forwardSfmlEventByImportance( Engine& engine, const sf::Event& sfEvent );
		void updateAllSystemsByImportance( Engine& engine, World& world );
	};
}
